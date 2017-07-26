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
	float getLowBodYtarg();
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
	char unk0[2048];
	__int32 ammo_type_maybe;
	char unk1[52];
	__int32 weapon_type;
	__int32 weapon_team;
	__int32 weapon_id;
	char unk2[4];
	float heat_ps;
	__int32 price;
	float m_fArmorRatio;
	float player_speed;
	float player_speed_alt;
	__int32 crosshair_min_distance;
	__int32 crosshair_delta;
	float m_fPenetration;
	__int32 m_iDamage;
	float m_fRange;
	float m_fRangeModifier;
	__int32 bullets;
	float cycle_time; 
	float cycle_time_alt;
	char unk3[416];
	float spread; 
	float spread_alt;
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