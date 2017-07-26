#include "Gamehooking.hpp"
#include "AimAssistance.hpp"

LineThroughSmokeFn LineThroughSmoke;

void AimAssist::work(UserCMD *userCMD, char *bSendPacket)
{
	setTarget(-1);
	bActive = false;

	this->userCMD = userCMD;
	this->bSendPacket = bSendPacket;

	if (!Global::locPlayer || Global::locPlayer->getLeben() <= 0)
		return;

	if (!Global::locPlayer->getWeapon() || csFuncs->isWeapNonAim(Global::locPlayer->getWeaponId()) || Global::locPlayer->getWeapon()->isEmpty())
		return;

	if (mVars.bEnableSkelet)
	{
		if ((GetAsyncKeyState(mVars.kAim) || GetAsyncKeyState(mVars.kAim2)) && !csFuncs->isWeapNonAim(Global::locPlayer->getWeaponId()))
		{
			getBestTarget();
			if (hasTarget())
				targetRegion();
		}			
	}
	/*if ((!(userCMD->buttons & IN_ATTACK) || !hasTarget()) && mVars.bEnableSkelet)
		getBestTarget();

	if (hasTarget() && (GetAsyncKeyState(mVars.kAim) || GetAsyncKeyState(mVars.kAim2)) && mVars.bEnableSkelet)
		targetRegion();*/

	if (mVars.bEnableTrig || (mVars.bEnableTrigWithKey && (mVars.kAim != 0 || mVars.kAim2 != 0)))
		trig();

	if (mVars.bEnableRK)
		rk();

	if (!mVars.bSilent)
		p_Engine->setViewAechse(userCMD->viewangles);
}

void AimAssist::getBestTarget()
{
	float maxDistancija = 8192.f;
	float najblizi;
	int iIndex = -1;

	float fov;

	if (mVars.maxFov == 0)
		fov = static_cast<float>(1);
	else
		fov = static_cast<float>(mVars.maxFov);

	int locPlyrFrdShots = Global::locPlayer->getFiredShots();

	for (int i = 1; i <= p_GlobVars->maxClients; i++)
	{
		Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

		if (!checkTarget(player))
			continue;

		Vector3 locViewAechse = userCMD->viewangles;

		Vector3 locVecAechse = Global::locPlayer->getVecAechse();
		Vector3 locViewOriginal = Global::locPlayer->getViewOriginal();
		Vector3 neu;
		neu = locViewOriginal + locVecAechse;

		Vector3 glava;

		if (locPlyrFrdShots > mVars.afterShots)
			glava = player->getSkeletById(realAimSpot[mVars.afterAim]);
		else if (locPlyrFrdShots < mVars.afterShots)
			glava = player->getSkeletById(realAimSpot[mVars.preAim]);

		float najblizi = csFuncs->getFoV(locViewAechse, csFuncs->racunajAechse(neu, glava));

		if (najblizi > fov)
			continue;

		float distancija = csFuncs->distance(Global::locPlayer->getViewOriginal(), player->getViewOriginal());

		if (fabs(fov - najblizi) < 5)
		{
			if (distancija < maxDistancija)
			{
				fov = najblizi;
				iIndex = i;
				maxDistancija = distancija;
			}
		}
		else if (najblizi < fov)
		{
			fov = najblizi;
			iIndex = i;
			maxDistancija = distancija;
		}
	}
	setTarget(iIndex);
}

void AimAssist::targetRegion()
{
	Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(iTarget));

	int locPlyrFrdShots = Global::locPlayer->getFiredShots();

	Vector3 locPlyrVekPunch = Global::locPlayer->getVektPunch();

	std::random_device r3nd0m;
	std::mt19937 r3nd0mGen(r3nd0m());

	std::uniform_real<float> r3nd0mXAechse(1.7f, 1.9f);
	std::uniform_real<float> r3nd0mYAechse(1.7f, 1.9f);

	locPlyrVekPunch.xAechse *= r3nd0mXAechse(r3nd0mGen);
	locPlyrVekPunch.yAechse *= r3nd0mYAechse(r3nd0mGen);
	locPlyrVekPunch.zAechse = 0.0f;

	csFuncs->normaleAechse(locPlyrVekPunch);

	Vector3 locViewAechse = userCMD->viewangles;

	Vector3 locVecAechse = Global::locPlayer->getVecAechse();
	Vector3 locViewOriginal = Global::locPlayer->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	Vector3 glava;

	if (locPlyrFrdShots > mVars.afterShots/* || csFuncs->isWeapPistol(Global::locPlayer->getWeaponId()) || Global::locPlayer->getWeaponId() == weapon_ssg08*/)
		glava = player->getSkeletById(realAimSpot[mVars.afterAim]);
	else if (locPlyrFrdShots < mVars.afterShots)
		glava = player->getSkeletById(realAimSpot[mVars.preAim]);

	Vector3 ugao;

	ugao = csFuncs->racunajAechse(neu, glava);
	ugao.xAechse -= locPlyrVekPunch.xAechse;
	ugao.yAechse -= locPlyrVekPunch.yAechse;

	if (mVars.umiriFactor != 0)
		csFuncs->umiri(locViewAechse, ugao, mVars.umiriFactor);

	csFuncs->normaleAechse(ugao);

	bActive = true;

	userCMD->viewangles = ugao;
}

bool AimAssist::checkTarget(Entity *player)
{
	if (!player)
		return false;

	if (player == Global::locPlayer)
		return false;

	if (player->isStationary())
		return false;

	if (player->getLeben() <= 0)
		return false;

	if (player->getTeam() == Global::locPlayer->getTeam())
		return false;

	int locPlayerFrdShots = Global::locPlayer->getFiredShots();

	Vector3 locVecAechse = Global::locPlayer->getVecAechse();
	Vector3 locViewOriginal = Global::locPlayer->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	Vector3 glava;

	if (locPlayerFrdShots > mVars.afterShots)
		glava = player->getSkeletById(realAimSpot[mVars.afterAim]);
	else if (locPlayerFrdShots < mVars.afterShots)
		glava = player->getSkeletById(realAimSpot[mVars.preAim]);

	if (!csFuncs->isVis(neu, glava, player))
		return false;

	if (isBehindSmoke(neu, glava))
		return false;

	return true;
}

bool AimAssist::isBehindSmoke(Vector3 loc, Vector3 rem)
{
	static bool initLGTS = false;
	if (!initLGTS)
	{
		offs.lgtSmoke = csFuncs->findPattern(XorStr("\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"), XorStr("xxxxxxxx????xxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0);
		initLGTS = true;
	}

	LineThroughSmoke = (LineThroughSmokeFn)(offs.lgtSmoke);

	return LineThroughSmoke(loc, rem, true);
}

Vector3 stariVekPunch;

void AimAssist::rk()
{
	int locPlyrFrdShots = Global::locPlayer->getFiredShots();

	if (userCMD->buttons & IN_ATTACK)
	{
		Vector3 locPlyrVekPunch = Global::locPlayer->getVektPunch();

		std::random_device r3nd0m;
		std::mt19937 r3nd0mGen(r3nd0m());

		std::uniform_real<float> r3nd0mXAechse(1.7f, 1.9f);
		std::uniform_real<float> r3nd0mYAechse(1.7f, 1.9f);

		locPlyrVekPunch.xAechse *= r3nd0mXAechse(r3nd0mGen);
		locPlyrVekPunch.yAechse *= r3nd0mYAechse(r3nd0mGen);
		locPlyrVekPunch.zAechse = 0.0f;

		csFuncs->normaleAechse(locPlyrVekPunch);

		if (locPlyrFrdShots > 2)
		{
			Vector3 viewAechse = userCMD->viewangles;

			Vector3 viewAechseMod = locPlyrVekPunch;

			viewAechseMod -= stariVekPunch;
			viewAechseMod.zAechse = 0.0f;

			csFuncs->normaleAechse(viewAechseMod);

			viewAechse -= viewAechseMod;
			viewAechse.zAechse = 0.0f;

			csFuncs->normaleAechse(viewAechse);

			userCMD->viewangles = viewAechse;
		}
		stariVekPunch = locPlyrVekPunch;
	}
	else
	{
		stariVekPunch.xAechse = stariVekPunch.yAechse = stariVekPunch.zAechse = 0;
	}
}

void AimAssist::trig()
{
	Vector3 neu, rem, forward;

	trace_t tr;
	Ray_t ray;

	TraceFilterSkipCurrentEntity filter;
	filter.pSkip = reinterpret_cast<void*>(Global::locPlayer);

	Vector3 viewAechse = userCMD->viewangles;

	viewAechse += Global::locPlayer->getVektPunch() * 2.f;

	csFuncs->praviVektor(viewAechse, forward);

	forward *= Global::locPlayer->getWeapon()->getWeapInfo()->m_fRange;

	Vector3 locVecAechse = Global::locPlayer->getVecAechse();
	Vector3 locViewOriginal = Global::locPlayer->getViewOriginal();
	neu = locViewOriginal + locVecAechse;

	rem = neu + forward;

	ray.Init(neu, rem);
	p_EngineTrace->TraceRay(ray, 0x46004003, (TraceFilter*)&filter, &tr);

	if (!tr.m_pEntityHit)
		return;

	bool dh = false;

	if (tr.hitgroup == HITGROUP_HEAD || tr.hitgroup == HITGROUP_CHEST || tr.hitgroup == HITGROUP_STOMACH || (tr.hitgroup == HITGROUP_LEFTARM || tr.hitgroup == HITGROUP_RIGHTARM) || (tr.hitgroup == HITGROUP_LEFTLEG || tr.hitgroup == HITGROUP_RIGHTLEG))
		dh = true;

	Entity *player = reinterpret_cast<Entity*>(tr.m_pEntityHit);
	if (player && !player->isStationary() && player->getLeben() > 0 && player->isPlayer())
	{
		if (player->getTeam() != Global::locPlayer->getTeam())
		{
			if (mVars.bEnableTrigWithKey)
			{
				if (dh && (!(userCMD->buttons & IN_ATTACK) && (!GetAsyncKeyState(mVars.kAim) != 1 || !GetAsyncKeyState(mVars.kAim2) != 1))) // if you don't attack currently
					userCMD->buttons |= IN_ATTACK;

				static bool pucao = false;
				if (csFuncs->isWeapPistol(Global::locPlayer->getWeaponId()))
				{
					if (userCMD->buttons & IN_ATTACK)
						if (pucao)
							userCMD->buttons &= ~IN_ATTACK;

					pucao = userCMD->buttons & IN_ATTACK ? true : false;
				}
			}
			else
			{
				if (dh && (!(userCMD->buttons & IN_ATTACK) && GetAsyncKeyState(VK_LBUTTON) != 1)) // if you don't attack currently
					userCMD->buttons |= IN_ATTACK;

				static bool pucao = false;
				if (csFuncs->isWeapPistol(Global::locPlayer->getWeaponId()))
				{
					if (userCMD->buttons & IN_ATTACK)
						if (pucao)
							userCMD->buttons &= ~IN_ATTACK;

					pucao = userCMD->buttons & IN_ATTACK ? true : false;
				}
			}
		}
	}
}