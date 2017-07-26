#include "Gamehooking.hpp"
#include "AimRage.hpp"

std::unordered_map<int, std::vector<const char*>> hitboxes =
{
	{ HITBOX_HEAD,{ "head_0" } },
	{ HITBOX_NECK,{ "neck_0" } },
	{ HITBOX_PELVIS,{ "pelvis" } },
	{ HITBOX_SPINE,{ "spine_0", "spine_1", "spine_2", "spine_3" } },
	{ HITBOX_LEGS,{ "leg_upper_L", "leg_upper_R", "leg_lower_L", "leg_lower_R", "ankle_L", "ankle_R" } },
	{ HITBOX_ARMS,{ "hand_L", "hand_R", "arm_upper_L", "arm_lower_L", "arm_upper_R", "arm_lower_R" } },
};

int iNajPlyr = -1;

void AimRage::work(UserCMD *userCMD, char *bSendPacket)
{
	dropTarget();

	this->userCMD = userCMD;
	this->bSendPacket = bSendPacket;

	if (!Global::locPlayer->getWeapon())
		return;

	if (csFuncs->isWeapNonAim(Global::locPlayer->getWeaponId()) || Global::locPlayer->getWeapon()->isEmpty())
		return;

	Global::bBotting = false;
	Global::bSearching = false;

	if (iNajPlyr == -1)
		getBestTarget();

	if (mVars.bEnableCekaj && !pressedKey[mVars.cekajTasta])
		return;

	if (iNajPlyr != -1 && !Global::locPlayer->getWeapon()->isReload() && bTarget)
		targetRegion();
}

void AimRage::dropTarget()
{
	//if (!checkTarget(iNajPlyr))
	iNajPlyr = -1;
}

bool AimRage::isLocalInTarget(Entity *player)
{
	Vector3 src, rem, forward;
	trace_t tr;

	if (!player)
		return false;

	Vector3 viewAechse = player->getEyeAechse();
	csFuncs->praviVektor(viewAechse, forward);
	forward *= 8142.f;

	src = player->getViewOriginal() + player->getVecAechse();
	rem = src + forward;

	traceIt(src, rem, MASK_SHOT, player, &tr);

	if (reinterpret_cast<Entity*>(tr.m_pEntityHit) == Global::locPlayer)
		return true;
	return false;
}

void AimRage::getBestTarget()
{
	najFov = mVars.hvhFov;
	maxDistancija = 8192.f;
	najPretnja = 0.f;
	najKost = Vector3(0, 0, 0);
	najPlyr = nullptr;
	bTarget = false;
	iNajPlyr = -1;

	for (int i = 1; i <= p_GlobVars->maxClients; i++)
	{
		if (!checkTarget(i))
			continue;

		Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

		Vector3 locViewAechse;
		p_Engine->getViewAechse(locViewAechse);
		Vector3 neu;
		neu = Global::locPlayer->getViewOriginal() + Global::locPlayer->getVecAechse();

		Vector3 hitBone = getHitBox(player, realHitboxSpot[mVars.hitAim]);

		Global::bSearching = true;

		if (mVars.bEnableTargetType == 0)
		{
			float najblizi = csFuncs->getFoV(locViewAechse, csFuncs->racunajAechse(neu, hitBone));

			if (najblizi < najFov)
			{
				najFov = najblizi;
				iNajPlyr = i;
				najPlyr = player;

				if (mVars.bHitscan)
				{
					if (getNajTacka(player, najKost))
						bTarget = true;
				}
				else if (mVars.bHitscanHitbox)
				{
					if (getNajHitbox(player, najKost))
						bTarget = true;
				}
				else
				{
					najKost = hitBone;
					bTarget = true;
				}
			}
		}

		if (mVars.bEnableTargetType == 1)
		{
			float najblizi = csFuncs->getFoV(locViewAechse, csFuncs->racunajAechse(neu, hitBone));
			float distancija = csFuncs->distance(Global::locPlayer->getViewOriginal(), player->getViewOriginal());

			if (distancija < maxDistancija && najblizi < najFov)
			{
				maxDistancija = distancija;
				iNajPlyr = i;
				najPlyr = player;

				if (mVars.bHitscan)
				{
					if (getNajTacka(player, najKost))
						bTarget = true;
				}
				else if (mVars.bHitscanHitbox)
				{
					if (getNajHitbox(player, najKost))
						bTarget = true;
				}
				else
				{
					najKost = hitBone;
					bTarget = true;
				}
			}
		}

		if (mVars.bEnableTargetType == 2)
		{
			float najblizi = csFuncs->getFoV(locViewAechse, csFuncs->racunajAechse(neu, hitBone));
			float distancija = csFuncs->distance(neu, hitBone);
			float zivot = (float)(player->getLeben());

			float pretnja = zivot / distancija;

			if (isLocalInTarget(player))
				pretnja += 100;

			if (pretnja > najPretnja && najblizi < najFov)
			{
				najPretnja = pretnja;
				iNajPlyr = i;
				najPlyr = player;

				if (mVars.bHitscan)
				{
					if (getNajTacka(player, najKost))
						bTarget = true;
				}
				else if (mVars.bHitscanHitbox)
				{
					if (getNajHitbox(player, najKost))
						bTarget = true;
				}
				else
				{
					najKost = hitBone;
					bTarget = true;
				}
			}
		}
	}
}

Vector3 AimRage::getHitBox(Entity *player, int hitbox)
{
	matrix3x4a_t matrix[128];

	if (!player->setupBones(matrix, 128, 0x00000100, 0))
		return Vector3(0, 0, 0);

	studiohdr_t *studioHdr = p_ModelInfo->GetStudioModel(player->getModel());

	if (!studioHdr)
		return Vector3(0, 0, 0);

	mstudiohitboxset_t *studioHitboxSet = studioHdr->pHitboxSet(player->getHitboxSet());
	mstudiobbox_t *untransformedBox = studioHitboxSet->pHitbox(hitbox);

	if (!untransformedBox)
		return Vector3(0, 0, 0);

	Vector3 min, max, center;

	VectorTransform(untransformedBox->bbmin, matrix[untransformedBox->bone], min);
	VectorTransform(untransformedBox->bbmax, matrix[untransformedBox->bone], max);

	center = (min + max) * 0.5;
	if (!center.xAechse && !center.yAechse && !center.zAechse)
		return Vector3(0, 0, 0);

	if (!center.xAechse && !center.yAechse && !center.zAechse)
		return Vector3(0, 0, 0);

	return center;
}

bool AimRage::getNajTacka(Entity *player, Vector3 &best_hitbox)
{
	for (std::unordered_map<int, std::vector<const char*>>::iterator itr = hitboxes.begin(); itr != hitboxes.end(); itr++)
	{
		if (!mVars.bWallbangBones[itr->first])
			continue;

		std::vector<const char*> hitboxList = hitboxes[itr->first];

		for (std::vector<const char*>::iterator itr2 = hitboxList.begin(); itr2 != hitboxList.end(); itr2++)
		{
			int kost = player->getSkeletByName(*itr2);
			Vector3 hitbox = player->getSkeletById(kost); //getHitBox(player, kost);

			if (hitbox == Vector3(0, 0, 0))
				return false;

			float dmg = getDamageVec(hitbox);

			if (dmg > mVars.wallbangDmg)
			{
				best_hitbox = hitbox;
				return true;
			}
		}
	}
	return false;
}

bool AimRage::getNajHitbox(Entity *player, Vector3 &best_hitbox)
{
	Vector3 hitbox = getHitBox(player, realHitboxSpot[mVars.hitAim]);

	if (hitbox == Vector3(0, 0, 0))
		return false;

	float dmg = getDamageVec(hitbox);

	if (dmg > mVars.wallbangDmg)
	{
		best_hitbox = hitbox;
		return true;
	}
	return false;
}

bool AimRage::checkTarget(int i)
{
	Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

	if (!player)
		return false;

	if (player == Global::locPlayer)
		return false;

	if (player->getTeam() == Global::locPlayer->getTeam())
		return false;

	if (player->isStationary())
		return false;

	if (player->isImmune())
		return false;

	if (player->getLeben() <= 0)
		return false;
	
	if (!mVars.bHitscan && !mVars.bHitscanHitbox)
	{
		if (mVars.wallbangDmg > getDamageVec(getHitBox(player, realHitboxSpot[mVars.hitAim])))
			return false;
	}

	return true;
}

template<class T, class U>
T AimRage::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float AimRage::lagFix()
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

void AimRage::targetRegion()
{
	//pucanjeCheck();

	Vector3 neu, ugao;
	neu = Global::locPlayer->getViewOriginal() + Global::locPlayer->getVecAechse();

	najKost = najPlyr->predict(najKost);

	ugao = csFuncs->racunajAechse(neu, najKost);
	ugao -= Global::locPlayer->getVektPunch() * 2.f;

	csFuncs->normaleAechse(ugao);

	Global::bBotting = true;

	userCMD->viewangles = ugao;

	autoStati();
	autoCucati();
	autoPucanje();

	if (!mVars.bSilent)
		p_Engine->setViewAechse(userCMD->viewangles);

	if (mVars.bEnableLagComp)
	{
		this->userCMD->tick_count = lagComp->fixTickcount(najPlyr);
	}
}

Vector3 tempAechse;
#define SPIN_REVS_PER_SECOND 15.0f
void AimRage::fixupAngleDifference(UserCMD *userCMD, char *bSendPacket)
{
	if (!Global::locPlayer->getWeapon() || csFuncs->isWeapNonAim(Global::locPlayer->getWeaponId()))
		return;

	float fTime;

	static bool bFlip;
	static float fDance = 0.0f;
	static float rYaw = 0.0f;

	bFlip = !bFlip;

	tempAechse = userCMD->viewangles;

	/*int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);*/
	if (Global::locPlayer->getMoveTip() == MOVETYPE_WALK /*&& !iHasShiftHeld*/ && !(userCMD->buttons & IN_USE) /*&& !(userCMD->buttons & IN_ATTACK) && !Global::bBotting*/)
	{
		if (mVars.bEnableTypeX != 0)
		{
			int typeX = mVars.bEnableTypeX;

			if (typeX == 1)
				tempAechse.xAechse = -89.0f;
			else if (typeX == 2)
				tempAechse.xAechse = 89.0f;
			else if (typeX == 3)
				tempAechse.xAechse = 180.0f;
			else if (typeX == 4)
				tempAechse.xAechse = 178.0f;
			else if (typeX == 5)
				tempAechse.xAechse = 991.0f;
			else if (typeX == 6)
				tempAechse.xAechse = -179.998992f;
			else if (typeX == 7)
				tempAechse.xAechse = 1080.0f;
			else if (typeX == 8)
			{
				fDance += 15.0f;

				if (fDance > 100)
					fDance = 0.0f;
				else if (fDance > 50.f)
					tempAechse.xAechse = 330.f;
				else if (fDance < 50.f)
					tempAechse.xAechse = 30.f;

			}
			else if (typeX == 9)
				tempAechse.xAechse = 0.0f;
		}

		if (mVars.bEnableTypeY != 0)
		{
			if (!mVars.bLbyBreaker)
			{
				int typeY = bFlip ? mVars.bEnableTypeY : mVars.bFakeTypeY;

				static float fYaw = 0.0f;
				static bool yFlip;

				if (bFlip)
					yFlip = !yFlip;
			
				if (typeY == 1)
				{
					fTime = p_GlobVars->curtime;

					tempAechse.yAechse = fmod(fTime * SPIN_REVS_PER_SECOND * 360.0f, 360.0f);
				}
				else if (typeY == 2)
					tempAechse.yAechse = yFlip ? 270.0f : 90.0f;
				else if (typeY == 3)
					if (yFlip)
						tempAechse.yAechse += 90.0f;
					else
						tempAechse.yAechse -= 90.0f;
				else if (typeY == 4)
					tempAechse.yAechse -= 180.0f;
				else if (typeY == 5)
					tempAechse.yAechse -= 0.0f;
				else if (typeY == 6)
					tempAechse.yAechse += 90.0f;
				else if (typeY == 7)
					tempAechse.yAechse -= 90.0f;
				else if (typeY == 8)
					tempAechse.yAechse = 0.0f;
			}
			else
			{
				// ~ deleted cause pub release
				// so here I have a meme for you:
				tempAechse.yAechse = bFlip ? 90.f : -180.0f;
			}
			*bSendPacket = bFlip;
		}

		//if (!Global::bBotting)
		userCMD->viewangles = tempAechse;

		csFuncs->normaleAechse(userCMD->viewangles);
	}
}

void AimRage::fixMovement(Vector3 &oang, UserCMD *userCMD)
{
	Vector3 vMove = Vector3(userCMD->forwardmove, userCMD->sidemove, 0.0f);
	float flSpeed = vMove.length();
	Vector3 qMove;
	csFuncs->vekAechse2(vMove, qMove);
	float normalized = fmod(userCMD->viewangles.yAechse + 180.f, 360.f) - 180.f;
	float normalizedx = fmod(userCMD->viewangles.xAechse + 180.f, 360.f) - 180.f;
	float flYaw = DEG2RAD((normalized - oang.yAechse) + qMove.yAechse);
	if (normalizedx >= 90.0f || normalizedx <= -90.0f) userCMD->forwardmove = -cos(flYaw) * flSpeed;
	else userCMD->forwardmove = cos(flYaw) * flSpeed;
	userCMD->sidemove = sin(flYaw) * flSpeed;
}

bool AimRage::hitSansa(Vector3 aechse, Entity *ent, float sansa)
{
	auto weap = Global::locPlayer->getWeapon();

	if (!weap)
		return false;

	Vector3 forward, right, up;
	Vector3 neu = Global::locPlayer->getVecAechse() + Global::locPlayer->getViewOriginal();
	csFuncs->aechseVektor(aechse, (float*)&forward, (float*)&right, (float*)&up);

	int cHits = 0;
	int cNeededHits = static_cast<int>(150.f * (sansa / 100.f));

	weap->upAccurPen();
	float weap_sir = weap->getSir();
	float weap_inac = weap->getInac();

	for (int i = 0; i < 150; i++)
	{
		//csFuncs->randomSeed(i + 1);

		float a = csFuncs->randomFloat(0.f, 1.f);
		float b = csFuncs->randomFloat(0.f, 2.f * M_PI_F);
		float c = csFuncs->randomFloat(0.f, 1.f);
		float d = csFuncs->randomFloat(0.f, 2.f * M_PI_F);

		float inac = a * weap_inac;
		float sir = c * weap_sir;

		if (*weap->getItemDefIdx() == 64)
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector3 sirVec((cos(b) * inac) + (cos(d) * sir), (sin(b) * inac) + (sin(d) * sir), 0), direction;

		direction.xAechse = forward.xAechse + (sirVec.xAechse * right.xAechse) + (sirVec.yAechse * up.xAechse);
		direction.yAechse = forward.yAechse + (sirVec.xAechse * right.yAechse) + (sirVec.yAechse * up.yAechse);
		direction.zAechse = forward.zAechse + (sirVec.xAechse * right.zAechse) + (sirVec.yAechse * up.zAechse);
		direction.normalize();

		Vector3 sirView;
		csFuncs->vekAechse(direction, up, sirView);
		sirView.normalize();

		Vector3 rem;
		csFuncs->praviVektor(aechse - (sirView - aechse), rem);

		trace_t tr;
		Ray_t ray;

		ray.Init(neu, (neu + (rem * weap->getWeapInfo()->m_fRange)));
		p_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, reinterpret_cast<IHandleEntity*>(ent), &tr);

		if (tr.m_pEntityHit == reinterpret_cast<void*>(ent))
			++cHits;

		if (static_cast<int>((static_cast<float>(cHits) / 150.f) * 100.f) >= sansa)
			return true;

		if ((150 - i + cHits) < cNeededHits)
			return false;
	}
	return false;
}

//bool AimRage::hitSansa(float sansa)
//{
//	auto weap = Global::locPlayer->getWeapon();
//
//	if (!weap)
//		return 0;
//
//	/*if (sansa >= 99.f)
//		sansa = 99.f;
//	if (sansa < 1.0f)
//		sansa = 1.0f;
//
//	float sir = weap->getSir();
//
//	return ((((100.f - sansa) * 0.65f) * 0.01125f) >= sir);*/
//}

void AimRage::autoPucanje()
{
	if (!mVars.bEnableAutoPucanje)
		return;

	auto weap = Global::locPlayer->getWeapon();

	if (!najPlyr || !weap || csFuncs->isWeapNonAim(Global::locPlayer->getWeaponId()) || Global::locPlayer->getWeapon()->isEmpty())
		return;

	if (userCMD->buttons & IN_USE)
		return;

	float nextPrimAtt = weap->getNextPrimAtt();
	float tick = Global::locPlayer->getTickBase() * p_GlobVars->interval_per_tick;

	if (nextPrimAtt > tick)
	{
		if (Global::locPlayer->getWeaponId() == weapon_revolver)
			userCMD->buttons &= ~IN_ATTACK2;
		else
			userCMD->buttons &= ~IN_ATTACK;
	}
	else
	{
		if (mVars.bEnableAutoZum && csFuncs->isWeapSniper(Global::locPlayer->getWeaponId()) && !Global::locPlayer->isZum())
			userCMD->buttons |= IN_ATTACK2;
		else if (Global::locPlayer->getWeaponId() == weapon_revolver)
			userCMD->buttons |= IN_ATTACK2;
		else
			if (hitSansa(userCMD->viewangles, najPlyr, mVars.hitsansaAmnt))
				userCMD->buttons |= IN_ATTACK;
	}
}

void AimRage::pucanjeCheck()
{
	auto weap = Global::locPlayer->getWeapon();

	float tick = Global::locPlayer->getTickBase() * p_GlobVars->interval_per_tick;
	float nextShot = weap->getNextPrimAtt() - tick;

	if (nextShot > 0)
		return;

	if (!(userCMD->buttons & IN_ATTACK))
		return;

	if (Global::locPlayer->getWeaponId() == weapon_c4)
		return;

	if (Global::locPlayer->getWeaponId() == weapon_revolver)
		userCMD->buttons &= ~IN_ATTACK2;
	else
		userCMD->buttons &= ~IN_ATTACK;
}

void AimRage::autoCucati()
{
	if (!mVars.bEnableCucati)
		return;

	if (!najPlyr)
		return;

	userCMD->buttons |= IN_DUCK;
}

void AimRage::autoStati()
{
	if (!mVars.bEnableStati)
		return;

	if (!najPlyr)
		return;

	userCMD->forwardmove = 0;
	userCMD->sidemove = 0;
	userCMD->upmove = 0;
}

bool AimRage::canPenetrate(WeapInfo_t *wpn_data, FireBulletData &data)
{
	using HandleBulletPenetrationFn = bool(__thiscall*)(Entity*, float&, int&, int*, trace_t*, Vector3*, float, float, float, int, int, float, int*, Vector3*, float, float, float*);
	static HandleBulletPenetrationFn BulletHandler;

	if (!BulletHandler)
		BulletHandler = (HandleBulletPenetrationFn)(csFuncs->findPattern(XorStr("\x55\x8B\xEC\x83\xEC\x70\x8B\x55\x00\x56\x8B\x75\x00\x89\x4D\x00\x32\xC9\x8B\x12\x57\x8A\x46\x00"), XorStr("xxxxxxxx?xxx?xx?xxxxxxx?"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0));

	int usestaticvalues = 0;
	surfacedata_t *enter_surface_data = p_PhysSurfProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int material = enter_surface_data->game.material;

	return BulletHandler(Global::locPlayer, wpn_data->m_fPenetration, material, &usestaticvalues, &data.enter_trace, &data.direction, 0.f, enter_surface_data->game.flPenetrationModifier, enter_surface_data->game.flDamageModifier, 0, 0x1002, wpn_data->m_fPenetration, &data.penetrate_count, &data.src, 0.f, 0.f, &data.current_damage);
}

float AimRage::getDamageVec(const Vector3 &vecPoint)
{
	float damage = 0.f;

	Vector3 rem = vecPoint;

	FireBulletData data;

	data.src = (Global::locPlayer->getViewOriginal() + Global::locPlayer->getVecAechse());
	data.filter.pSkip = Global::locPlayer;

	Vector3 ugao = csFuncs->racunajAechse(data.src, rem);
	csFuncs->praviVektor(ugao, data.direction);

	data.direction.normalize();

	EntityWeapon *weap = Global::locPlayer->getWeapon();
	if (simulateFireBullet(weap, data))
		damage = data.current_damage;

	return damage;
}

bool AimRage::simulateFireBullet(EntityWeapon *weap, FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	WeapInfo_t *weaponData = Global::locPlayer->getWeapon()->getWeapInfo();

	if (weaponData == NULL)
		return false;

	data.current_damage = (float)weaponData->m_iDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->m_fRange - data.trace_length;

		Vector3 end = data.src + data.direction * data.trace_length_remaining;

		traceIt(data.src, end, MASK_SHOT | CONTENTS_GRATE, Global::locPlayer, &data.enter_trace);
		clipTraceToPlayers(data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->m_fRangeModifier, data.trace_length * 0.002);

			Entity *player = reinterpret_cast<Entity*>(data.enter_trace.m_pEntityHit);
			if (player->getTeam() == Global::locPlayer->getTeam())
				return false;

			scaleDamage(data.enter_trace.hitgroup, player, weaponData->m_fArmorRatio, data.current_damage);

			return true;
		}

		if (!handleBulletPenetration(weaponData, data)/*canPenetrate(weaponData, data)*/)
			break;
	}

	return false;
}

bool AimRage::handleBulletPenetration(WeapInfo_t *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = p_PhysSurfProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((DWORD)enter_surface_data + 76);

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->m_fRangeModifier), (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector3 dummy;
	trace_t trace_exit;

	/*
	// from memory
	if (!traceToExit(dummy, data.enter_trace, data.enter_trace.endpos.xAechse, data.enter_trace.endpos.yAechse, data.enter_trace.endpos.zAechse, data.direction.xAechse, data.direction.yAechse, data.direction.zAechse, &trace_exit))
		return false;
	*/

	if (!traceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = p_PhysSurfProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)((DWORD)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_fPenetration) * 1.25f);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool AimRage::traceToExit(Vector3 &end, trace_t &tr, float x, float y, float z, float x2, float y2, float z2, trace_t *trace)
{
	static DWORD TraceToExit = csFuncs->findPattern(XorStr("\x55\x8B\xEC\x83\xEC\x2C\xF3\x0F\x10\x75\x00\x33\xC0"), XorStr("xxxxxxxxxx?xx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0);

	if (!TraceToExit)
		return false;

	_asm
	{
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}
}

bool AimRage::traceToExit(Vector3 &end, trace_t *enter_trace, Vector3 start, Vector3 dir, trace_t *exit_trace)
{
	auto distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = p_EngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		traceIt(end, new_end, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

		if (exit_trace->m_pEntityHit == nullptr)
			return false;

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			traceIt(end, start, 0x600400B, reinterpret_cast<Entity*>(exit_trace->m_pEntityHit), exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
		{
			if (exit_trace->m_pEntityHit)
			{
				if ((enter_trace->m_pEntityHit != nullptr && enter_trace->m_pEntityHit != p_ClientEntList->GetClientEntity(0)) && isBreakableEntity(reinterpret_cast<Entity*>(enter_trace->m_pEntityHit)))
					return true;
			}
			continue;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace->surface.flags >> 7) & 1))
			continue;
		
		if (DotProduct(exit_trace->plane.normal, dir) <= 1.0f)
		{
			float fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}
	return false;
}

bool AimRage::isBreakableEntity(Entity *ent)
{
	// ~ deleted cause pub release
	// but here you have a signature based IsBreakableEntity func :)

	typedef bool(__thiscall *isBreakbaleEntityFn)(Entity*);
	static isBreakbaleEntityFn IsBreakableEntity;

	if (!IsBreakableEntity)
		IsBreakableEntity = (isBreakbaleEntityFn)csFuncs->findPattern(XorStr("\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68"), XorStr("xxxxxxxxxxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0);

	return IsBreakableEntity(ent);
}

void AimRage::clipTraceToPlayers(const Vector3 &vecAbsStart, const Vector3 &vecAbsEnd, unsigned int mask, TraceFilter *filter, trace_t *tr)
{
	// ~ deleted cause pub release
	// but here you have a signature based ClipTraceToPlayers func :)
	// oh wait.. https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/util_shared.cpp#L687

	static DWORD ClipTraceToPlayers = csFuncs->findPattern(XorStr("\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10"), XorStr("xxxxxxxxxxxxxxxxxxxxxxxx????xxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0);

	if (!ClipTraceToPlayers)
		return;

	_asm
	{
		mov eax, filter
		lea ecx, tr
		push ecx
		push eax
		push mask
		lea edx, vecAbsEnd
		lea ecx, vecAbsStart
		call ClipTraceToPlayers
		add esp, 0xC
	}
}

bool AimRage::DidHitNonWorldEntity(Entity *ent)
{
	return ent != NULL && ent == reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(0));
}

void AimRage::scaleDamage(int hitgroup, Entity *player, float weapon_armor_ratio, float &current_damage)
{
	bool heavArmor = player->getHeavArm();
	int armor = player->getArmVal();

	switch (hitgroup)
	{
	case HITGROUP_HEAD:

		if (heavArmor)
			current_damage *= (current_damage * 4.f) * 0.5f;
		else
			current_damage *= 4.f;

		break;

	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:

		current_damage *= 1.f;
		break;

	case HITGROUP_STOMACH:

		current_damage *= 1.25f;
		break;

	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:

		current_damage *= 0.75f;
		break;
	}

	if (isArmored(player, armor, hitgroup))
	{
		float v47 = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_armor_ratio * 0.5f;

		if (heavArmor)
		{
			armor_bonus_ratio = 0.33f;
			armor_ratio = (weapon_armor_ratio * 0.5f) * 0.5f;
			v47 = 0.33f;
		}

		float new_damage = current_damage * armor_ratio;

		if (heavArmor)
			new_damage *= 0.85f;

		if (((current_damage - (current_damage * armor_ratio)) * (v47 * armor_bonus_ratio)) > armor)
			new_damage = current_damage - (armor / armor_bonus_ratio);

		current_damage = new_damage;
	}
}

bool AimRage::isArmored(Entity *player, int armorVal, int hitgroup)
{
	bool res = false;

	if (armorVal > 0)
	{
		switch (hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:

			res = true;
			break;

		case HITGROUP_HEAD:

			res = player->getHelm();
			break;

		}
	}

	return res;
}

void AimRage::traceIt(Vector3 &vecAbsStart, Vector3 &vecAbsEnd, unsigned int mask, Entity *ign, trace_t *tr)
{
	Ray_t ray;

	TraceFilterSkipCurrentEntity filter;
	filter.pSkip = reinterpret_cast<void*>(ign);

	ray.Init(vecAbsStart, vecAbsEnd);

	p_EngineTrace->TraceRay(ray, mask, &filter, tr);
}

void MoveFixer::start()
{
	aechse = Global::userCMD->viewangles;
	forward = Global::userCMD->forwardmove;
	right = Global::userCMD->sidemove;
}

void MoveFixer::stop()
{
	float yaw_delta = Global::userCMD->viewangles.yAechse - aechse.yAechse;
	float f1;
	float f2;

	if (aechse.yAechse < 0.f)
		f1 = 360.0f + aechse.yAechse;
	else
		f1 = aechse.yAechse;

	if (Global::userCMD->viewangles.yAechse < 0.0f)
		f2 = 360.0f + Global::userCMD->viewangles.yAechse;
	else
		f2 = Global::userCMD->viewangles.yAechse;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	Global::userCMD->forwardmove = cos(DEG2RAD(yaw_delta)) * forward + cos(DEG2RAD(yaw_delta + 90.f)) * right;
	Global::userCMD->sidemove = sin(DEG2RAD(yaw_delta)) * forward + sin(DEG2RAD(yaw_delta + 90.f)) * right;
}