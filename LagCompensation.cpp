#include "harCs.hpp"
#include "LagCompensation.hpp"

LagCompensation::LagCompensation()
{
	
}

void LagCompensation::logEntity(Entity *player)
{
	int idx = player->getIdx();
	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	if (!player || !(player->getLeben() > 0))
	{
		for (int i = 0; i < 11; i++)
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}

	float simTime = player->getSimulTime();

	if (!isValidTick(simTime))
		return;

	int highestRecordIdx = -1;
	float highestSimTime = 0.0f;

	for (int i = 0; i < 11; i++)
	{
		if (m_LagRecords[i].m_fSimulationTime > simTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;

		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;

		if (m_LagRecords[i].m_fSimulationTime == simTime)
			return;

		if (m_LagRecords[i].m_fSimulationTime > highestSimTime)
		{
			highestRecordIdx = i;
			highestSimTime = m_LagRecords[i].m_fSimulationTime;
		}
	}

	highestRecordIdx++;
	highestRecordIdx = highestRecordIdx % 11;

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(offs.invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	m_LagRecords[highestRecordIdx].m_vecOrigin = player->getAbsOriginal();
	m_LagRecords[highestRecordIdx].m_fSimulationTime = player->getSimulTime();
	m_LagRecords[highestRecordIdx].m_angEyeAngles = player->getAbsAechse();
	m_LagRecords[highestRecordIdx].flags = player->getFlags();
	m_LagRecords[highestRecordIdx].m_flLowerBodyYawTarget = player->getLowBodYtarg();

	for (int i = 0; i < 24; i++)
		m_LagRecords[highestRecordIdx].m_flPoseParameter[i] = player->getPoseParams(i);

	m_LagRecords[highestRecordIdx].headSpot = player->getSkeletById(8);
}

void LagCompensation::logCurrentEnt(Entity *player)
{
	int idx = player->getIdx();

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(offs.invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	this->m_PrevRecords[idx].m_vecOrigin = player->getAbsOriginal();
	this->m_PrevRecords[idx].m_fSimulationTime = player->getSimulTime();
	this->m_PrevRecords[idx].m_angEyeAngles = player->getAbsAechse();
	this->m_PrevRecords[idx].flags = player->getFlags();
	this->m_PrevRecords[idx].m_flLowerBodyYawTarget = player->getLowBodYtarg();

	for (int i = 0; i < 24; i++)
		this->m_PrevRecords[idx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::setEntity(Entity *player, LagRecord record)
{
	if (!isValidTick(record.m_fSimulationTime))
		return;

	this->logCurrentEnt(player);

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(offs.invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	player->setAbsOriginal(record.m_vecOrigin);
	player->setAbsAechse(record.m_angEyeAngles);
	player->getFlags() = record.flags;
	player->getLowBodYtarg() = record.m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		player->getPoseParams(i) = record.m_flPoseParameter[i];

	player->updateClientSideAnimation();
}

void LagCompensation::setCurrentEnt(Entity *player)
{
	int idx = player->getIdx();

	if (!isValidTick(m_PrevRecords[idx].m_fSimulationTime))
		return;

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(offs.invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	player->setAbsOriginal(m_PrevRecords[idx].m_vecOrigin);
	player->setAbsAechse(m_PrevRecords[idx].m_angEyeAngles);
	player->getFlags() = m_PrevRecords[idx].flags;
	player->getLowBodYtarg() = m_PrevRecords[idx].m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		player->getPoseParams(i) = m_PrevRecords[idx].m_flPoseParameter[i];

	player->updateClientSideAnimation();
}

template<class T, class U>
T LagCompensation::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float LagCompensation::lerpTime()
{
	int ud_rate = p_Console->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = p_Console->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = p_Console->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = p_Console->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = p_Console->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = p_Console->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = p_Console->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

void LagCompensation::fakeLagFix(Entity *player, int historyIdx)
{
	int idx = player->getIdx();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord &recentLR = m_LagRecords[historyIdx];
	LagRecord prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];

	if (recentLR.m_fSimulationTime == 0.0f)
		return;

	if (!isValidTick(recentLR.m_fSimulationTime))
		return;

	if ((recentLR.m_vecOrigin - prevLR.m_vecOrigin).lengthSqr() > 4096.f)
	{
		for (auto i = 0; i <= player->getChockedTicks(); i++)
		{
			// ~ deleted parts cause pub release

			rebGameMovement->FullWalkMove(player); // resimulate
		}
		recentLR.m_bIsBreakingLagComp = true;
		//p_Console->ConsoleColorPrintf(Color::Green(), "m_bIsBreakingLagComp: true\n");

		return;
	}

	setEntity(player, recentLR);
}

void LagCompensation::initLagRecord()
{
	for (int i = 0; i <= 32; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			m_LagRecord[i][j].m_fSimulationTime = 0.0f;
		}
	}
}

int LagCompensation::fixTickcount(Entity *player)
{
	int idx = player->getIdx();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord &recentLR = m_LagRecords[mVars.historyIdx];

	if (!isValidTick(recentLR.m_fSimulationTime))
		return TIME_TO_TICKS(player->getSimulTime() + lerpTime());
	
	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;

	return iTargetTickCount;
}

bool LagCompensation::isValidTick(float simTime)
{
	INetChannelInfo *nci = p_Engine->getNetChannelInfo();

	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(lerpTime());

	int predCmdArrivTick = p_GlobVars->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(lerpTime())));

	return abs(flCorrect) < 0.2f;
}

void LagCompensation::log(ClientFrameStage_en stage)
{
	if (!p_Engine->isIG())
		return;

	if (!Global::locPlayer || Global::locPlayer == NULL)
		return;

	if (mVars.bEnableLagComp)
	{
		if (stage == ClientFrameStage_en::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			for (int i = 1; i < p_GlobVars->maxClients; i++)
			{
				Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				if (!player || player == nullptr)
					continue;

				if (player == Global::locPlayer)
					continue;

				if (player->isStationary())
					continue;

				if (player->isImmune())
					continue;

				if (player->getLeben() <= 0)
					continue;

				if (player->getTeam() == Global::locPlayer->getTeam())
					continue;

				this->logEntity(player);
			}
		}
	}
}