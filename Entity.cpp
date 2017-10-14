#include "harCs.hpp"
#include "Entity.hpp"

int Entity::getTeam()
{
	return *(int*)((DWORD)this + offs.entTeam);
}

bool Entity::isPlayer()
{
	Entity *ent = NULL;

	for (int i = 0; i < 64; i++)
	{
		ent = reinterpret_cast<Entity*>(csFuncs->getEntityById(i));

		if (ent == this)
			return true;
	}
	return false;
}

DWORD Entity::getGlowIndex()
{
	return *(DWORD*)((DWORD)this + offs.entGlowIndex);
}

bool Entity::isStationary()
{
	return *(bool*)((DWORD)this + offs.stationary);
}

bool Entity::isImmune()
{
	return *(bool*)((DWORD)this + offs.immun);
}

bool Entity::getLifeState()
{
	return *(bool*)((DWORD)this + offs.lifeSt);
}

int &Entity::getFlags()
{
	return *(int*)((DWORD)this + offs.flags);
}

int Entity::getMoveTip()
{
	return *(int*)((DWORD)this + 0x258);
}

int Entity::getFiredShots()
{
	return *(int*)((DWORD)this + offs.metakFired);
}

int Entity::getTickBase()
{
	return *(int*)((DWORD)this + offs.tickBase);
}

int Entity::getArmVal()
{
	return *(int*)((DWORD)this + offs.armVal);
}

bool Entity::getHelm()
{
	return *(bool*)((DWORD)this + offs.hHelm);
}

bool Entity::getHeavArm()
{
	return *(bool*)((DWORD)this + offs.hHeavArmor);
}

Vector3 Entity::getVel()
{
	return *(Vector3*)((DWORD)this + offs.vekVel);
}

Vector3 Entity::predict(Vector3 v0)
{
	return (v0 + (this->getVel() * p_GlobVars->interval_per_tick));
}

Vector3 Entity::getVektPunch()
{
	return *(Vector3*)((DWORD)this + offs.aimPunch);
}

Vector3 Entity::getVecAechse()
{
	return *(Vector3*)((DWORD)this + offs.vecAechse);
}

Vector3 Entity::getViewOriginal()
{
	return *(Vector3*)((DWORD)this + offs.viewOriginal);
}

int Entity::getCompRank(int idx)
{
	DWORD tmp = *(DWORD*)(csFuncs->dClientDll + offs.gameRes);
	return *(int*)((DWORD)tmp + offs.compRank + (int)idx * 4);
}

int Entity::getCompWins(int idx)
{
	DWORD tmp = *(DWORD*)(csFuncs->dClientDll + offs.gameRes);
	return *(int*)((DWORD)tmp + offs.compWins + (int)idx * 4);
}

bool Entity::getGameResConnected(int idx)
{
	DWORD tmp = *(DWORD*)(csFuncs->dClientDll + offs.gameRes);
	return *(bool*)((DWORD)tmp + offs.resConnected + idx);
}

int Entity::getGameResTeam(int idx)
{
	DWORD tmp = *(DWORD*)(csFuncs->dClientDll + offs.gameRes);
	return *(int*)((DWORD)tmp + offs.resTeam + (int)idx * 4);
}

std::wstring Entity::getName(int idx)
{
	DWORD tmpBase = *(DWORD*)(csFuncs->dClientDll + offs.radar);
	DWORD tmp = *(DWORD*)(tmpBase + 0x54);

	wchar_t *wcName = (wchar_t*)(tmp + 0x1E0 * idx + 0x24);
	std::wstring name(wcName);

	return name;
}

int Entity::getLeben()
{
	return *(int*)((DWORD)this + offs.leben);
}

Vector3 Entity::getSkeletById(int idx)
{
	matrix3x4_t kostToWorldOut[128];
	if (this->setupBones(kostToWorldOut, 128, 0x100, 0))
		return Vector3(kostToWorldOut[idx][0][3], kostToWorldOut[idx][1][3], kostToWorldOut[idx][2][3]);
	else
		return Vector3(0, 0, 0);
}

int Entity::getSkeletByName(const char *imeKost)
{
	studiohdr_t *studioHdr = p_ModelInfo->GetStudioModel(this->getModel());

	if (!studioHdr)
		return -1;

	matrix3x4_t kostToWorldOut[128];

	if (!this->setupBones(kostToWorldOut, 128, 256, 0))
		return -1;

	for (int i = 0; i < studioHdr->numbones; i++)
	{
		mstudiobone_t *studioKost = studioHdr->pBone(i);

		if (!studioKost)
			continue;

		if (studioKost->pszName() && strcmp(studioKost->pszName(), imeKost) == 0)
			return i;
	}

	return -1;
}

EntityWeapon *Entity::getWeapon()
{
	DWORD weap = *(DWORD*)((DWORD)this + offs.activWeap);
	auto plyrWeap = p_ClientEntList->GetClientEntityFromHandle(weap);

	if (plyrWeap)
		return reinterpret_cast<EntityWeapon*>(plyrWeap);
	else
		return NULL;
}

UINT *Entity::getWeapons()
{
	return (UINT*)((DWORD)this + 0x2DE8);
}

int Entity::getWeaponId()
{
	DWORD weap = *(DWORD*)((DWORD)this + offs.activWeap);
	auto plyrWeap = p_ClientEntList->GetClientEntityFromHandle(weap);

	if (plyrWeap)
		return *(int*)(reinterpret_cast<EntityWeapon*>(plyrWeap) + offs.itmDI);
	else
		return 0;
}

int *Entity::getModelIdx()
{
	return (int*)((DWORD)this + offs.modIdx);
}

DWORD Entity::getViewModel()
{
	return *(DWORD*)((DWORD)this + offs.viewModel);
}

Client *Entity::getClientCls()
{
	void *netwable = (void*)((DWORD)(this) + 0x8);
	typedef Client*(__thiscall *o_GetClientCls)(void*);
	return VT::vfunc<o_GetClientCls>(netwable, 2)(netwable);
}

int Entity::getIdx()
{
	void *netwable = (void*)((DWORD)(this) + 0x8);
	typedef int(__thiscall *o_GetIdx)(void*);
	return VT::vfunc<o_GetIdx>(netwable, 10)(netwable);
}

model_t *Entity::getModel()
{
	return *(model_t**)((DWORD)this + 0x6C);
}

bool Entity::setupBones(matrix3x4_t *kostToWorldOut, int maxKosti, int kostMaska, float curtime)
{
	//*(int*)((DWORD)(this) + offs.iCheckForOccl) = p_GlobVars->framecount;
	//*(int*)((DWORD)(this) + 0xA28) = 0;

	//*(int*)((DWORD)(this) + offs.upisivaKost) = 0;
	//*(int*)((DWORD)(this) + offs.iCheckForOccl) = p_GlobVars->framecount;
	//reinterpret_cast<void(__fastcall*)(void*)>(offs.invalKostCache)(this);

	void *rendable = (void*)((DWORD)(this) + 0x4);
	typedef bool(__thiscall *o_setupBones)(void*, matrix3x4_t*, int, int, float);
	return VT::vfunc<o_setupBones>(rendable, 13)(rendable, kostToWorldOut, maxKosti, kostMaska, curtime);
}

void AngleIMatrix(const QAngle& angles, matrix3x4_t& matrix)
{
	Assert(s_bMathlibInitialized);
	float		sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[0][1] = cp*sy;
	matrix[0][2] = -sp;
	matrix[1][0] = sr*sp*cy + cr*-sy;
	matrix[1][1] = sr*sp*sy + cr*cy;
	matrix[1][2] = sr*cp;
	matrix[2][0] = (cr*sp*cy + -sr*-sy);
	matrix[2][1] = (cr*sp*sy + -sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.f;
	matrix[1][3] = 0.f;
	matrix[2][3] = 0.f;
}

bool Entity::setupBonesEx(Entity *player, int kostMaska, matrix3x4_t *kostToWorldOut)
{
	//void *rendable = (void*)((DWORD)player + 0x4);

	//if (!rendable)
	//	return false;

	//studiohdr_t *studiohdr = p_ModelInfo->GetStudioModel(player->getModel());
	//if (!studiohdr)
	//	return false;

	//// 9876 boneaccessor
	//// 9884 oldWritableBones m_WritableBones 
	//// 9880 oldReadableBones m_ReadableBones 

	//matrix3x4_t *backup_matrix = *(matrix3x4_t**)((DWORD)rendable + 0x2694);
	//if (!backup_matrix)
	//	return false;

	//int oldReadableBones = *(int*)((DWORD)rendable + 9880);

	//int bonesMaskNeedRecalc = kostMaska | oldReadableBones;

	//// raw
	//Vector3 origin = lagComp->m_LagRecord[player->getIdx()][mVars.historyIdx].m_vAbsOrigin;
	//Vector3 angles = lagComp->m_LagRecord[player->getIdx()][mVars.historyIdx].m_vecAngles;

	//// backup interpolated data
	//Vector3 backup_absorigin = player->getAbsOriginal();
	//Vector3 backup_absangles = player->getAbsAechse();

	//float backup_poses[24];
	//for (int i = 0; i < 24; i++)
	//	backup_poses[i] = player->getPoseParams(i);

	//matrix3x4_t parentTransform;
	//AngleIMatrix(angles, parentTransform);
	//parentTransform[0][3] = origin.xAechse;
	//parentTransform[1][3] = origin.yAechse;
	//parentTransform[2][3] = origin.zAechse;

	//player->setAbsOriginal(origin);
	//player->setAbsAechse(angles);
	//for (int i = 0; i < 24; i++)
	//	*(float*)((DWORD)player + offs.poseParams + sizeof(float) * i) = lagComp->m_LagRecord[player->getIdx()][mVars.historyIdx].m_flPoseParameter[i];

	//*(int*)((DWORD)rendable + 224) |= 8;//AddFlag( EFL_SETTING_UP_BONES ); (1 << 3));

	//Vector3 pos[128];
	//Quaternion q[128];

	//memset(pos, 0xFF, sizeof(pos));
	//memset(q, 0xFF, sizeof(q));

	//player->standardBlendingRules(studiohdr, pos, q, p_GlobVars->curtime, bonesMaskNeedRecalc);

	//*(matrix3x4_t**)((DWORD)rendable + 0x2694) = kostToWorldOut;

	//byte computed[0x100] = { 0 };
	//player->buildTransformations(studiohdr, pos, q, parentTransform, bonesMaskNeedRecalc, computed);

	//*(int*)((DWORD)rendable + 224) &= ~(8);//AddFlag( EFL_SETTING_UP_BONES ); (1 << 3));

	//*(matrix3x4_t**)((DWORD)rendable + 0x2694) = backup_matrix;

	//player->setAbsOriginal(backup_absorigin);
	//player->setAbsAechse(backup_absangles);
	//for (int i = 0; i < 24; i++)
	//	*(float*)((DWORD)player + offs.poseParams + sizeof(float) * i) = backup_poses[i];

	return true;
}

Collideable *Entity::getCollideable()
{
	return (Collideable*)((DWORD)this + offs.coll);
}

DWORD Entity::getRgflCoordinateFrame()
{
	return offs.rgflCoordinateFrame;
}

Vector3 *Entity::getHeadRot()
{
	return (Vector3*)((DWORD)this + offs.angRot);
}

float &Entity::getLowBodYtarg()
{
	return *(float*)((DWORD)this + offs.lowBodYtarg);
}

int Entity::getHitboxSet()
{
	return *(int*)((DWORD)this + offs.hitboxSet);
}

Vector3 &Entity::getEyeAechse()
{
	return *(Vector3*)((DWORD)this + offs.angEyeAechse);
}

bool Entity::isAlive()
{
	return (bool)(*(int*)((DWORD)this + offs.lifeSt) == 0);
}

bool Entity::isZum()
{
	return *(bool*)((DWORD)this + offs.uZum);
}

int Entity::getOwner()
{
	return *(int*)((DWORD)this + offs.owner);
}

float &Entity::getSimulTime()
{
	return *(float*)((DWORD)this + offs.simulTime);
}

float &Entity::getCycle()
{
	return *(float*)((DWORD)this + offs.cycle);
}
int &Entity::getSequence()
{
	return *(int*)((DWORD)this + offs.sequence);
}

Vector3 &Entity::getAbsOriginal()
{
	typedef Vector3&(__thiscall *o_getAbsOriginal)(void*);
	return VT::vfunc<o_getAbsOriginal>(this, 10)(this);
}

Vector3 &Entity::getNetOriginal()
{
	static int offset = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flFriction")) - sizeof(Vector3);
	return *(Vector3*)((DWORD)this + offset);
}

Vector3 &Entity::getAbsAechse()
{
	typedef Vector3&(__thiscall *o_getAbsAechse)(void*);
	return VT::vfunc<o_getAbsAechse>(this, 11)(this);
}

void Entity::setAbsOriginal(Vector3 origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector3 &origin);
	static SetAbsOriginFn SetAbsOrigin;
	
	if (!SetAbsOrigin)
		SetAbsOrigin = (SetAbsOriginFn)(csFuncs->findPattern(XorStr("\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00"), XorStr("xxxxxxxxxxxxx??"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0));

	SetAbsOrigin(this, origin);
}

void Entity::setAbsAechse(Vector3 aechse)
{
	using SetAbsAechseFn = void(__thiscall*)(void*, const Vector3 &aechse);
	static SetAbsAechseFn SetAbsAechse;

	if (!SetAbsAechse)
		SetAbsAechse = (SetAbsAechseFn)(csFuncs->findPattern(XorStr("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8"), XorStr("xxxxxxxxxxxxxxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0));

	SetAbsAechse(this, aechse);
}

Vector3 const &Entity::getRenderOrigin()
{
	void *rendable = (void*)((DWORD)(this) + 0x4);
	typedef Vector3&(__thiscall *o_GetRenderOrigin)(void*);
	return VT::vfunc<o_GetRenderOrigin>(rendable, 1)(rendable);
}

QAngle const &Entity::getRenderAechse()
{
	void *rendable = (void*)((DWORD)(this) + 0x4);
	typedef QAngle&(__thiscall *o_GetRenderAechse)(void*);
	return VT::vfunc<o_GetRenderAechse>(rendable, 2)(rendable);
}

float &Entity::getPoseParams(int idx)
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + offs.poseParams + sizeof(float) * idx);
}

void *Entity::getObsTarget()
{
	return (void*)((DWORD)this + offs.obsTrgt);
}

int Entity::getChockedPackets()
{
	if (this->getSimulTime() > Global::oldSimulTime[this->getIdx()])
		return TIME_TO_TICKS(fabs(this->getSimulTime() - Global::oldSimulTime[this->getIdx()]));
	return 0;
}

int Entity::getChockedTicks()
{
	if (this->getSimulTime() > this->getOldSimulTime())
		return TIME_TO_TICKS(fabs(this->getSimulTime() - this->getOldSimulTime()));
	return 0;
}

float &Entity::getSurfaceFriction()
{
	static int offs = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_surfaceFriction"));
	return *(float*)((DWORD)this + offs);
}

float &Entity::getStepSize()
{
	static int offs = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_flStepSize"));
	return *(float*)((DWORD)this + offs);
}

float &Entity::getMaxSpeed()
{
	static int offs = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_flMaxSpeed"));
	return *(float*)((DWORD)this + offs);
}

float &Entity::getFallVel()
{
	static int offs = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_flFallVelocity"));
	return *(float*)((DWORD)this + offs);
}

float &Entity::getAnimTime()
{
	return *(float*)((DWORD)this + offs.animTime);
}

float &Entity::getOldSimulTime()
{
	static int offs = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_flSimulationTime")) + 0x4;
	return *(float*)((DWORD)this + offs);
}

int &Entity::getDucked()
{
	return *(int*)((DWORD)this + offs.ducked);
}

int &Entity::getDucking()
{
	return *(int*)((DWORD)this + offs.ducking);
}

float &Entity::getDuckAmnt()
{
	return *(float*)((DWORD)this + offs.duckAmnt);
}

float &Entity::getDuckSpeed()
{
	return *(float*)((DWORD)this + offs.duckSpeed);
}

void Entity::standardBlendingRules(studiohdr_t *studiohdr, Vector3 *pos, Quaternion *q, float curtime, int kostMaska)
{
	typedef void(__thiscall *o_standardBlendingRules)(void*, studiohdr_t*, Vector3*, Quaternion*, float, int);
	VT::vfunc<o_standardBlendingRules>(this, 200)(this, studiohdr, pos, q, curtime, kostMaska);
}

void Entity::buildTransformations(studiohdr_t *studiohdr, Vector3 *pos, Quaternion *q, const matrix3x4_t &cameraTransform, int kostMaska, byte *computed)
{
	typedef void(__thiscall *o_buildTransformations)(void*, studiohdr_t*, Vector3*, Quaternion*, const matrix3x4_t &, int, byte*);
	VT::vfunc<o_buildTransformations>(this, 184)(this, studiohdr, pos, q, cameraTransform, kostMaska, computed);
}

void Entity::updateClientSideAnimation()
{
	typedef void(__thiscall *o_updateClientSideAnimation)(void*);
	VT::vfunc<o_updateClientSideAnimation>(this, 218)(this);
}

DWORD *Entity::getVarMap()
{
	return nullptr;
}

float EntityWeapon::getAccurPen()
{
	return *(float*)((DWORD)this + offs.accPen);
}

float EntityWeapon::getNextPrimAtt()
{
	return *(float*)((DWORD)this + offs.nextPrimAtt);
}

float EntityWeapon::getInac()
{
	typedef float(__thiscall *o_getInac)(void*);
	return VT::vfunc<o_getInac>(this, 469)(this);
}

float EntityWeapon::getSir()
{
	typedef float(__thiscall *o_getSir)(void*);
	return VT::vfunc<o_getSir>(this, 439)(this);
}

void EntityWeapon::upAccurPen()
{
	typedef void(__thiscall *o_upAccurPen)(void*);
	VT::vfunc<o_upAccurPen>(this, 470)(this);
}

bool EntityWeapon::isEmpty()
{
	int clip = *(int*)((DWORD)this + offs.clip);
	return clip == 0;
}

bool EntityWeapon::isReload()
{
	return *(bool*)((DWORD)this + 0x3235);
}

WeapInfo_t *EntityWeapon::getWeapInfo()
{
	if (!this || this == NULL)
		return NULL;

	typedef WeapInfo_t *(__thiscall *o_getWeapInfo)(void*);
	return VT::vfunc<o_getWeapInfo>(this, 446)(this);
}

int *EntityWeapon::getItemDefIdx()
{
	return (int*)((DWORD)this + offs.itmDI);
}

int *EntityWeapon::getItemIdHigh()
{
	return (int*)((DWORD)this + offs.itemIdHigh);
}

char *EntityWeapon::getCustName()
{
	return (char*)((DWORD)this + offs.custName);
}

int *EntityWeapon::getFallbackPK()
{
	return (int*)((DWORD)this + offs.fallbackPK);
}

int *EntityWeapon::getFallbackS()
{
	return (int*)((DWORD)this + offs.fallbackS);
}

float *EntityWeapon::getFallbackW()
{
	return (float*)((DWORD)this + offs.fallbackW);
}

int *EntityWeapon::getFallbackST()
{
	return (int*)((DWORD)this + offs.fallbackST);
}

int *EntityWeapon::getAccId()
{
	return (int*)((DWORD)this + offs.accId);
}