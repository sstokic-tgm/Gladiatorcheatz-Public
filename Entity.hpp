#pragma once

//#include "harCs.hpp"

class WeapInfo_t;
class Entity;
class EntityWeapon;
class Collideable;

enum MoveTip
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

class Entity
{

public:

	int getTeam();
	bool isPlayer();
	DWORD getGlowIndex();
	bool isStationary();
	bool isImmune();
	bool getLifeState();
	int &getFlags();
	int getMoveTip();
	int getFiredShots();
	int getTickBase();
	int getArmVal();
	bool getHelm();
	bool getHeavArm();
	Vector3 getVel();
	Vector3 predict(Vector3 v0);
	Vector3 getVektPunch();
	Vector3 getVecAechse();
	Vector3 getViewOriginal();
	int getCompRank(int idx);
	int getCompWins(int idx);
	bool getGameResConnected(int idx);
	int getGameResTeam(int idx);
	std::wstring getName(int idx);
	int getLeben();
	Vector3 getSkeletById(int idx);
	int getSkeletByName(const char *imeKost);
	EntityWeapon *getWeapon();
	UINT *getWeapons();
	int getWeaponId();
	int *getModelIdx();
	DWORD getViewModel();
	Client *getClientCls();
	int getIdx();
	model_t *getModel();
	bool setupBones(matrix3x4_t *kostToWorldOut, int maxKosti, int kostMaska, float curtime);
	bool setupBonesEx(Entity *player, int kostMaska, matrix3x4_t *kostToWorldOut);
	Collideable *getCollideable();
	DWORD getRgflCoordinateFrame();
	Vector3 *getHeadRot();
	float &getLowBodYtarg();
	int getHitboxSet();
	Vector3 &getEyeAechse();
	bool isAlive();
	bool isZum();
	int getOwner();
	float &getSimulTime();
	float &getCycle();
	int &getSequence();
	Vector3 &getAbsOriginal();
	Vector3 &getNetOriginal();
	Vector3 &getAbsAechse();
	void setAbsOriginal(Vector3 origin);
	void setAbsAechse(Vector3 aechse);
	Vector3 const &getRenderOrigin();
	QAngle const &getRenderAechse();
	float &getPoseParams(int idx);
	void *getObsTarget();
	int getChockedPackets();
	int getChockedTicks();
	float &getSurfaceFriction();
	float &getStepSize();
	float &getMaxSpeed();
	float &getFallVel();
	float &getAnimTime();
	float &getOldSimulTime();
	int &getDucked();
	int &getDucking();
	float &getDuckAmnt();
	float &getDuckSpeed();
	void standardBlendingRules(studiohdr_t *studiohdr, Vector3 *pos, Quaternion *q, float curtime, int kostMaska);
	void buildTransformations(studiohdr_t *studiohdr, Vector3 *pos, Quaternion *q, const matrix3x4_t &cameraTransform, int kostMaska, byte *computed);
	void updateClientSideAnimation();
	DWORD *getVarMap();
};

class WeapInfo_t
{
public:
	char pad00[0xC8];
	__int32 weapon_type;
	char padCC[0x20];
	__int32 m_iDamage;
	float m_fArmorRatio;
	char padF4[0x4];
	float m_fPenetration;
	char padFC[0x8];
	float m_fRange;
	float m_fRangeModifier;
	char pad10C[0x10];
	bool m_bHasSilencer;
};

class EntityWeapon
{

public:

	float getAccurPen();
	float getNextPrimAtt();
	float getInac();
	float getSir();
	void upAccurPen();
	bool isEmpty();
	bool isReload();
	WeapInfo_t *getWeapInfo();
	int *getItemDefIdx();
	int *getItemIdHigh();
	char *getCustName();
	int *getFallbackPK();
	int *getFallbackS();
	float *getFallbackW();
	int *getFallbackST();
	int *getAccId();
};


class Collideable
{

public:

	virtual void unk0();
	virtual Vector3 &vecMins() const;
	virtual Vector3 &vecMax() const;
};