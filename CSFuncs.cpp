#include "CSFuncs.hpp"

Offsets offs;
NetMngr::Mngr *netMngr = nullptr;

DWORD CSFuncs::findPattern(char *pattern, char *mask, DWORD start, DWORD end, DWORD offset)
{
	int patternLength = strlen(mask);
	bool found = false;

	for (DWORD i = start; i < end - patternLength; i++)
	{
		found = true;

		for (int idx = 0; idx < patternLength; idx++)
		{
			if (*(mask + idx) == 'x' && *(pattern + idx) != *(char*)(i + idx))
			{
				found = false;
				break;
			}
		}

		if (found)
			return i + offset;
	}
	return NULL;
}

bool CSFuncs::findOffset(DWORD moduleBase, DWORD moduleSize, char *pattern, char *mask, DWORD addressOffset, char *offsetName, DWORD *offsetAddress, bool subtractBase)
{
	DWORD addr = this->findPattern(pattern, mask, moduleBase, moduleBase + moduleSize, addressOffset);

	if (addr)
	{
		*offsetAddress = *(DWORD*)addr;

		if (subtractBase)
			*offsetAddress -= moduleBase;

		return true;
	}
	return false;
}

void CSFuncs::getOffsets(DWORD clientDll, DWORD clientDllSize, DWORD engineDll, DWORD engineDllSize)
{
	bool offsetsComplete = true;

	offs.metakFired = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_iShotsFired"));
	offs.coravDur = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flFlashDuration"));
	offs.compRank = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_iCompetitiveRanking"));
	offs.compWins = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_iCompetitiveWins"));
	offs.skelet = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_nForceBone"));
	offs.skelet += 0x1C;
	offs.leben = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_iHealth"));
	offs.flags = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_fFlags"));
	offs.lifeSt = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_lifeState"));
	offs.viewOriginal = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_vecOrigin"));
	offs.entTeam = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_iTeamNum"));
	offs.vecAechse = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_vecViewOffset[0]"));
	offs.activWeap = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_hActiveWeapon"));
	offs.itmDI = netMngr->getOffs(XorStr("CBaseCombatWeapon"), XorStr("m_iItemDefinitionIndex"));
	offs.c4Time = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_flC4Blow"));
	offs.c4Tick = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_bBombTicking"));
	offs.c4Def = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_bBombDefused"));
	offs.coll = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_Collision"));
	offs.collGrp = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_CollisionGroup"));
	offs.accPen = netMngr->getOffs(XorStr("CWeaponCSBase"), XorStr("m_fAccuracyPenalty"));
	offs.armVal = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_ArmorValue"));
	offs.hHelm = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bHasHelmet"));
	offs.hHeavArmor = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bHasHeavyArmor"));
	offs.tickBase = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_nTickBase"));
	offs.vekVel = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_vecVelocity[0]"));
	offs.nextPrimAtt = netMngr->getOffs(XorStr("CBaseCombatWeapon"), XorStr("m_flNextPrimaryAttack"));
	offs.clip = netMngr->getOffs(XorStr("CBaseCombatWeapon"), XorStr("m_iClip1"));
	offs.angRot = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_angEyeAngles[0]"));
	offs.lowBodYtarg = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flLowerBodyYawTarget"));
	offs.resConnected = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_bConnected"));
	offs.resTeam = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_iTeam"));
	offs.hitboxSet = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_nHitboxSet"));
	offs.immun = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bGunGameImmunity"));
	offs.angEyeAechse = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_angEyeAngles"));
	offs.aimPunch = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_aimPunchAngle"));
	offs.viewPunch = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_viewPunchAngle"));
	offs.uZum = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bIsScoped"));
	offs.weapons = (netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_hMyWeapons")) / 2);
	offs.modIdx = netMngr->getOffs(XorStr("CBaseViewModel"), XorStr("m_nModelIndex"));
	offs.viewModel = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_hViewModel[0]"));
	offs.viewModelIdx = netMngr->getOffs(XorStr("CBaseCombatWeapon"), XorStr("m_iViewModelIndex"));
	offs.accId = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_iAccountID"));
	offs.itemIdHigh = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_iItemIDHigh"));
	offs.custName = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_szCustomName"));
	offs.fallbackPK = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_nFallbackPaintKit"));
	offs.fallbackS = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_nFallbackSeed"));
	offs.fallbackW = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_flFallbackWear"));
	offs.fallbackST = netMngr->getOffs(XorStr("CBaseAttributableItem"), XorStr("m_nFallbackStatTrak"));
	offs.owner = netMngr->getOffs(XorStr("CBaseViewModel"), XorStr("m_hOwner"));
	offs.simulTime = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_flSimulationTime"));
	offs.cycle = netMngr->getOffs(XorStr("CBaseAnimating"), XorStr("m_flCycle"));
	offs.sequence = netMngr->getOffs(XorStr("CBaseAnimating"), XorStr("m_nSequence"));
	offs.poseParams = netMngr->getOffs(XorStr("CBaseAnimating"), XorStr("m_flPoseParameter"));
	offs.animTime = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_flAnimTime"));
	offs.obsTrgt = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_hObserverTarget"));
	offs.upisivaKost = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_nForceBone"));
	offs.upisivaKost += 0x20;
	offs.deadFlag4 = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("deadflag")) + 4;
	offs.duckSpeed = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flDuckSpeed"));
	offs.duckAmnt = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flDuckAmount"));
	offs.ducked = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bDucked"));
	offs.ducking = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_bDucking"));

	offs.rgflCoordinateFrame = offs.collGrp - 0x30;

	offs.entGlowIndex = offs.coravDur + 0x18;

	if (!this->findLocalPlayer(clientDll, clientDllSize))
		offsetsComplete = false;

	if (!this->findEntityList(clientDll, clientDllSize))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\xE8\x00\x00\x00\x00\x83\xC4\x04\xB8\x00\x00\x00\x00\xC3\xCC"), XorStr("x????xxxx????xx"), 9, XorStr("glowObjectBase"), &offs.glowObjectBase, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x56\x57\x8B\xF9\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00\x8B\x0D"), XorStr("xxxxxx????xx"), 6, XorStr("inCross"), &offs.inCross))
		offsetsComplete = false;

	if (!this->findOffset(engineDll, engineDllSize, XorStr("\xA1\x00\x00\x00\x00\x32\xC9\x8B\xB0\x00\x00\x00\x00\xC7\x80\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x0D"), XorStr("x????xxxx????xx????????x????xx"), 1, XorStr("engPtr"), &offs.engPtr, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7"), XorStr("xx????xxxx????xx"), 2, XorStr("gameRes"), &offs.gameRes, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\xA1\x00\x00\x00\x00\x8B\x0C\xB0\x8B\x01\xFF\x50\x00\x46\x3B\x35\x00\x00\x00\x00\x7C\xEA\x8B\x0D"), XorStr("x????xxxxxxx?xxx????xxxx"), 1, XorStr("radar"), &offs.radar, true))
		offsetsComplete = false;

	offs.revealAllRank = this->findPattern(XorStr("\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00"), XorStr("xxxxx????x????"), dClientDll, dClientDll + dClientDllSize, 0);
	offs.isRdy = this->getIsRdy();
	offs.glowMngr = *(DWORD*)(this->findPattern(XorStr("\x0F\x11\x05\x00\x00\x00\x00\x83\xC8\x01\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00"), XorStr("xxx????xxxxx????xxxx"), dClientDll, dClientDll + dClientDllSize, 0) + 3);
	offs.smokeCnt = *(DWORD*)(this->findPattern(XorStr("\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"), XorStr("xxxxxxxx????xxx"), dClientDll, dClientDll + dClientDllSize, 0) + 0x8);
	offs.iCheckForOccl = *(DWORD*)(csFuncs->findPattern(XorStr("\xA1\x00\x00\x00\x00\x8B\xB7\x00\x00\x00\x00\x89\x75\xF8"), XorStr("x????xx????xxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0) + 0x7);
	offs.invalKostCache = csFuncs->findPattern(XorStr("\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81"), XorStr("xx?????xxx????xxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0);
}

bool CSFuncs::findLocalPlayer(DWORD clientDll, DWORD clientDllSize)
{
	DWORD addr = this->findPattern(XorStr("\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x00"), XorStr("xxx????xx????xxxxx?"), clientDll, clientDll + clientDllSize, 0);

	if (addr)
	{
		offs.localPlayer = *(DWORD*)(addr + 3) + *(char*)(addr + 18) - clientDll;

		return true;
	}
	return false;
}

bool CSFuncs::findEntityList(DWORD clientDll, DWORD clientDllSize)
{
	DWORD addr = this->findPattern(XorStr("\x05\x00\x00\x00\x00\xC1\xE9\x00\x39\x48\x04"), XorStr("x????xx?xxx"), clientDll, clientDll + clientDllSize, 0);

	if (addr)
	{
		offs.entityList = *(DWORD*)(addr + 1) + *(char*)(addr + 7) - clientDll;

		return true;
	}
	return false;
}

void CSFuncs::init()
{
	do
	{
		hClientDll = GetModuleHandleA(XorStr("client.dll"));
		hEngineDll = GetModuleHandleA(XorStr("engine.dll"));

	} while (hClientDll == NULL && hEngineDll == NULL);

	GetModuleInformation(GetCurrentProcess(), hClientDll, &modClientDll, sizeof(MODULEINFO));
	GetModuleInformation(GetCurrentProcess(), hEngineDll, &modEngineDll, sizeof(MODULEINFO));

	dClientDll = (DWORD)modClientDll.lpBaseOfDll;
	dEngineDll = (DWORD)modEngineDll.lpBaseOfDll;
	dClientDllSize = modClientDll.SizeOfImage;
	dEngineDllSize = modEngineDll.SizeOfImage;

	netMngr = new NetMngr::Mngr;
	netMngr->init();

	this->getOffsets(dClientDll, dClientDllSize, dEngineDll, dEngineDllSize);
}

CSFuncs::~CSFuncs()
{
	delete netMngr;
}

DWORD CSFuncs::getClientMode()
{
	return this->findPattern(XorStr("\x8B\x0D\x00\x00\x00\x00\x8B\x01\x5D\xFF"), XorStr("xx????xxxx"), dClientDll, dClientDll + dClientDllSize, 2);
}

DWORD CSFuncs::getIsRdy()
{
	return this->findPattern(XorStr("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE"), XorStr("xxxxxxxxxxxx????xxx"), dClientDll, dClientDll + dClientDllSize, 0);
}

DWORD CSFuncs::getD3D()
{
	HMODULE hShader;
	DWORD dShaderDll, dShaderDllSize;
	MODULEINFO modShaderDll;

	do
	{
		hShader = GetModuleHandleA(XorStr("shaderapidx9.dll"));

	} while (hShader == NULL);

	GetModuleInformation(GetCurrentProcess(), hShader, &modShaderDll, sizeof(MODULEINFO));
	dShaderDll = (DWORD)modShaderDll.lpBaseOfDll;
	dShaderDllSize = modShaderDll.SizeOfImage;

	return **(DWORD**)(this->findPattern(XorStr("\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C"), XorStr("x????xxxxxx"), dShaderDll, dShaderDll + dShaderDllSize, 0) + 1);
}

DWORD CSFuncs::getRenderView()
{
	return (this->findPattern(XorStr("\xFF\x50\x14\xE8\x00\x00\x00\x00\x5D"), XorStr("xxxx????x"), dClientDll, dClientDll + dClientDllSize, 0) - 7);
}

DWORD CSFuncs::getLocalPlayer()
{
	return *(DWORD*)(dClientDll + offs.localPlayer);
}

DWORD CSFuncs::getEntityList()
{
	return dClientDll + offs.entityList;
}

DWORD CSFuncs::getEngine()
{
	return *(DWORD*)(dEngineDll + offs.engPtr);
}

int CSFuncs::getLocPlyrConnect(DWORD base)
{
	return *(int*)(base + 0x108);
}

DWORD CSFuncs::getEntityById(DWORD id)
{
	DWORD entityList = this->getEntityList();

	if (!entityList)
		return NULL;

	return *(DWORD*)(entityList + offs.entityLoopDist * id);
}

DWORD CSFuncs::getGlowObjects()
{
	return *(DWORD*)(dClientDll + offs.glowObjectBase);
}

DWORD CSFuncs::getGlowObjectCount()
{
	return *(DWORD*)(dClientDll + offs.glowObjectBase + offs.glowObjectCount);
}

DWORD CSFuncs::getInCross()
{
	DWORD locPlayer = this->getLocalPlayer();

	if (!locPlayer)
		return NULL;

	return *(DWORD*)(locPlayer + offs.inCross);
}

DWORD CSFuncs::getGameRes()
{
	return *(DWORD*)(dClientDll + offs.gameRes);
}

char *CSFuncs::getWeapName(int idx)
{
	switch (idx)
	{
	case weapon_none:
		return "Unknown";
	case weapon_deagle:
		return "Desert Eagle";
	case weapon_elite:
		return "Dual Berettas";
	case weapon_fiveseven:
		return "Five-SeveN";
	case weapon_glock:
		return "Glock-18";
	case weapon_ak47:
		return "AK-47";
	case weapon_aug:
		return "AUG";
	case weapon_awp:
		return "AWP";
	case weapon_famas:
		return "FAMAS";
	case weapon_g3sg1:
		return "G3SG1";
	case weapon_galilar:
		return "Galil AR";
	case weapon_m249:
		return "M249";
	case weapon_m4a1:
		return "M4A4";
	case weapon_mac10:
		return "MAC-10";
	case weapon_p90:
		return "P90";
	case weapon_ump45:
		return "UMP-45";
	case weapon_xm1014:
		return "XM1014";
	case weapon_bizon:
		return "PP-Bizon";
	case weapon_mag7:
		return "MAG-7";
	case weapon_negev:
		return "Negev";
	case weapon_sawedoff:
		return "Sawed-Off";
	case weapon_tec9:
		return "Tec-9";
	case weapon_taser:
		return "Zeus x27";
	case weapon_hkp2000:
		return "P2000";
	case weapon_mp7:
		return "MP7";
	case weapon_mp9:
		return "MP9";
	case weapon_nova:
		return "Nova";
	case weapon_p250:
		return "P250";
	case weapon_scar20:
		return "SCAR-20";
	case weapon_sg553:
		return "SG 553";
	case weapon_ssg08:
		return "SSG 08";
	case weapon_knife:
		return "Knife";
	case weapon_flashbang:
		return "Flashbang";
	case weapon_hegrenade:
		return "High Explosive Grenade";
	case weapon_smokegrenade:
		return "Smoke Grenade";
	case weapon_molotov:
		return "Molotov";
	case weapon_decoy:
		return "Decoy Grenade";
	case weapon_incgrenade:
		return "Incendiary Grenade";
	case weapon_c4:
		return "C4 Explosive";
	case weapon_knife_t:
		return "Knife";
	case weapon_m4a1_silencer:
		return "M4A1-S";
	case weapon_usp_silencer:
		return "USP-S";
	case weapon_cz75a:
		return "CZ75-Auto";
	case weapon_revolver:
		return "R8 Revolver";
	case weapon_knife_bayonet:
		return "Bayonet";
	case weapon_knife_flip:
		return "Flip Knife";
	case weapon_knife_gut:
		return "Gut Knife";
	case weapon_knife_karambit:
		return "Karambit";
	case weapon_knife_m9_bayonet:
		return "M9 Bayonet";
	case weapon_knife_tactical:
		return "Huntsman Knife";
	case weapon_knife_falchion:
		return "Falchion Knife";
	case weapon_knife_survival_bowie:
		return "Bowie Knife";
	case weapon_knife_butterfly:
		return "Butterfly Knife";
	case weapon_knife_push:
		return "Shadow Daggers";
	default:
		return "Unknown";
	}
}

bool CSFuncs::isWeapPistol(int idx)
{
	return(idx == weapon_deagle || idx == weapon_elite || idx == weapon_fiveseven || idx == weapon_glock
		|| idx == weapon_p250 || idx == weapon_usp_silencer || idx == weapon_tec9 || idx == weapon_taser || idx == weapon_hkp2000
		|| idx == weapon_cz75a || idx == weapon_revolver);
}

bool CSFuncs::isWeapSniper(int idx)
{
	return (idx == weapon_awp || idx == weapon_ssg08 || idx == weapon_scar20 || idx == weapon_g3sg1 || idx == weapon_sg553);
}

bool CSFuncs::isWeapNonAim(int idx)
{
	return (idx == weapon_c4 || idx == weapon_knife || idx == weapon_knife_bayonet || idx == weapon_knife_butterfly || idx == weapon_knife_falchion
		|| idx == weapon_knife_flip || idx == weapon_knife_gut || idx == weapon_knife_karambit || idx == weapon_knife_m9_bayonet || idx == weapon_knife_push
		|| idx == weapon_knife_survival_bowie || idx == weapon_knife_t || idx == weapon_knife_tactical || idx == weapon_flashbang || idx == weapon_hegrenade
		|| idx == weapon_smokegrenade || idx == weapon_molotov || idx == weapon_decoy || idx == weapon_incgrenade);
}

void CSFuncs::randomSeed(int iSeed)
{
	static auto ranSeed = reinterpret_cast<void(*)(int)>(GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomSeed")));
	if (ranSeed)
		ranSeed(iSeed);
}

float CSFuncs::randomFloat(float min, float max)
{
	static auto ranFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomFloat")));
	if (ranFloat)
		return ranFloat(min, max);
	else
		return 0.f;
}

int CSFuncs::randomInt(int min, int max)
{
	static auto ranInt = reinterpret_cast<int(*)(int, int)>(GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomInt")));
	if (ranInt)
		return ranInt(min, max);
	else
		return 0;
}

void CSFuncs::normaleAechse(Vector3 &vektRein)
{
	if (!std::isfinite(vektRein.xAechse))
		vektRein.xAechse = 0;
	if (!std::isfinite(vektRein.yAechse))
		vektRein.yAechse = 0;

	while (vektRein.yAechse < -180.0f)
		vektRein.yAechse += 360.0f;
	while (vektRein.yAechse > 180.0f)
		vektRein.yAechse -= 360.0f;

	if (vektRein.xAechse > 89.0f)
		vektRein.xAechse = 89.0f;
	if (vektRein.xAechse < -89.0f)
		vektRein.xAechse = -89.0f;

	vektRein.zAechse = 0;
}

float CSFuncs::distance(Vector3 loc, Vector3 rem)
{
	return (float)sqrt(pow(double(loc.xAechse - rem.xAechse), 2.0) + pow(double(loc.yAechse - rem.yAechse), 2.0) + pow(double(loc.zAechse - rem.zAechse), 2.0));
}

Vector3 CSFuncs::racunajAechse(Vector3 src, Vector3 dst)
{
	Vector3 ugao;
	Vector3 delta;

	delta = src - dst;

	this->vekAechse(delta, ugao);

	/*double hyp = sqrt(pow(delta.xAechse, 2.0f) + pow(delta.yAechse, 2.0f));

	ugao.xAechse = (float)(atanf(delta.zAechse / hyp) * 57.295779513082f);
	ugao.yAechse = (float)(atanf(delta.yAechse / delta.xAechse) * 57.295779513082f);
	ugao.zAechse = 0.0f;*/

	delta.normalize();

	return ugao;
}

void CSFuncs::vekAechse(Vector3 &forward, Vector3 &aechse)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		aechse[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		aechse[1] = 0.0f;
	}
	else
	{
		aechse[0] = atan2(-forward[2], forward.length2d()) * -180 / M_PI;
		aechse[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (aechse[1] > 90) aechse[1] -= 180;
		else if (aechse[1] < 90) aechse[1] += 180;
		else if (aechse[1] == 90) aechse[1] = 0;
	}

	aechse[2] = 0.0f;
}

void CSFuncs::vekAechse2(Vector3 &forward, Vector3 &aechse)
{
	Vector3 vecView;
	if (forward[1] == 0.f && forward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(forward[1], forward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);

		vecView[0] = atan2(forward[2], forward[2]) * 180.f / M_PI;
	}

	aechse[0] = -vecView[0];
	aechse[1] = vecView[1];
	aechse[2] = 0.f;
}

void CSFuncs::vekAechse(Vector3 &forward, Vector3 &up, Vector3 &aechse)
{
	Vector3 left = CrossProduct(up, forward);
	left.normalizeInPlace();

	float forwardDist = forward.length2d();

	if (forwardDist > 0.001f)
	{
		aechse.xAechse = atan2f(-forward.zAechse, forwardDist) * 180 / M_PI_F;
		aechse.yAechse = atan2f(forward.yAechse, forward.xAechse) * 180 / M_PI_F;

		float upZ = (left.yAechse * forward.xAechse) - (left.xAechse * forward.yAechse);
		aechse.zAechse = atan2f(left.zAechse, upZ) * 180 / M_PI_F;
	}
	else
	{
		aechse.xAechse = atan2f(-forward.zAechse, forwardDist) * 180 / M_PI_F;
		aechse.yAechse = atan2f(-left.xAechse, left.yAechse) * 180 / M_PI_F;
		aechse.zAechse = 0;
	}
}

void CSFuncs::aechseVektor(Vector3 aechsen, float *forward, float *right, float *up)
{
	float aechse;
	float sp, sy, cp, cy;

	aechse = aechsen[0] * (3.14159265358979323846f / 180);
	sp = sin(aechse);
	cp = cos(aechse);

	aechse = aechsen[1] * (3.14159265358979323846f / 180);
	sy = sin(aechse);
	cy = cos(aechse);

	if (forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}

	if (right || up)
	{
		float sr, cr;

		aechse = aechsen[2] * (3.14159265358979323846f / 180);
		sr = sin(aechse);
		cr = cos(aechse);

		if (right)
		{
			right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
			right[1] = -1 * sr * sp * sy + -1 * cr *cy;
			right[2] = -1 * sr * cp;
		}

		if (up)
		{
			up[0] = cr * sp *cy + -sr * -sy;
			up[1] = cr * sp *sy + -sr * cy;
			up[2] = cr * cp;
		}
	}
}

void CSFuncs::praviVektor(Vector3 src, Vector3 &dst)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(src[1]), &sy, &cy);
	SinCos(DEG2RAD(src[0]), &sp, &cp);

	dst.xAechse = cp*cy;
	dst.yAechse = cp*sy;
	dst.zAechse = -sp;
}

float SafeAcos(float x)
{
	if (x < -1.0) x = -1.0;
	else if (x > 1.0) x = 1.0;
	return acos(x);
}

float CSFuncs::getFoV(const Vector3 &viewAechse, const Vector3 &viewUgao)
{
	Vector3 aechse, ziel;

	this->praviVektor(viewAechse, ziel);
	this->praviVektor(viewUgao, aechse);

	return RAD2DEG(acos(DotProduct(ziel, aechse) / ziel.lengthSqr()));

	/*float fov;
	Vector3 ugao, ziel;

	ugao = this->racunaj(src, dst);
	this->praviVektor(aechse, ziel);
	this->praviVektor(ugao, ugao);

	float s = sqrt(pow(ziel.xAechse, 2.0f) + pow(ziel.yAechse, 2.0f) + pow(ziel.zAechse, 2.0f));
	float d = sqrt(pow(ziel.xAechse, 2.0f) + pow(ziel.yAechse, 2.0f) + pow(ziel.zAechse, 2.0f));

	float dot = (ziel.xAechse*ugao.xAechse) + (ziel.yAechse*ugao.yAechse) + (ziel.zAechse*ugao.zAechse);

	//fov = acos(dot / (s*d)) * (180.0f / 3.14159265358979323846f);
	fov = SafeAcos(dot / (s*d)) * (180.0f / 3.14159265358979323846f);
	return fov;*/
}

void CSFuncs::umiri(Vector3 src, Vector3 &dst, float factor)
{
	Vector3 delta;
	delta = dst - src;

	this->normaleAechse(delta);

	dst = src + delta / factor;
}

bool CSFuncs::isVis(Vector3 loc, Vector3 rem, Entity *base)
{
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (locPlyr && locPlyr->getLeben() > 0)
	{
		trace_t tr;
		Ray_t ray;
		TraceFilterSkipCurrentEntity filter;
		filter.pSkip = reinterpret_cast<void*>(locPlyr);

		ray.Init(loc, rem);
		p_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		return tr.m_pEntityHit == reinterpret_cast<void*>(base);
	}
	return false;
}

CntDwnTimer::CntDwnTimer()
{
	this->timestmp = -1.0f;
	this->dur = 0.0f;
	this->timeLength = 0.0f;
}

void CntDwnTimer::start(float dur)
{
	this->timestmp = this->now() + dur;
	this->dur = dur;
	this->timeLength = this->timestmp - this->now();
}

void CntDwnTimer::reset()
{
	this->timestmp = -1.0f;
	this->timeLength = 0.0f;
}

bool CntDwnTimer::isElapsed()
{
	if ((this->now() > this->timestmp))
	{
		this->timestmp = -1.0f;
		return true;
	}
	else
		return false;
}

float CntDwnTimer::getRemainingTime() const
{
	return (this->timestmp - this->now());
}

float CntDwnTimer::now() const
{
	return p_GlobVars->curtime;
}

float CntDwnTimer::getPercentge() const
{
	return ((this->now() - this->timestmp) / (this->timestmp + this->dur - this->timestmp)) * 100 * -1;
}

float CntDwnTimer::getTimeLength() const
{
	return timeLength;
}