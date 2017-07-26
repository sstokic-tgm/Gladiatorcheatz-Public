#pragma once

class WeapInfo_t;
class Entity;
class EntityWeapon;

struct FireBulletData
{
	Vector3 src;
	trace_t enter_trace;
	Vector3 direction;
	TraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class MoveFixer
{

public:

	void start();
	void stop();

private:

	float forward;
	float right;
	Vector3 aechse;
};

class AimRage
{

public:

	void work(UserCMD *userCMD, char *bSendPacket);
	void fixupAngleDifference(UserCMD *userCMD, char *bSendPacket);
	void fixMovement(Vector3 &oang, UserCMD *userCMD);

private:

	void dropTarget();
	bool isLocalInTarget(Entity *player);
	void getBestTarget();
	Vector3 getHitBox(Entity *player, int hitbox);
	bool getNajTacka(Entity *player, Vector3 &best_hitbox);
	bool getNajHitbox(Entity *player, Vector3 &best_hitbox);
	bool checkTarget(int i);

	template<class T, class U>
	T clamp(T in, U low, U high);

	float lagFix();
	void targetRegion();
	//bool hitSansa(float sansa);
	bool hitSansa(Vector3 aechse, Entity *ent, float sansa);

	void autoPucanje();
	void pucanjeCheck();
	void autoStati();
	void autoCucati();

	bool canPenetrate(WeapInfo_t *wpn_data, FireBulletData &data);
	float getDamageVec(const Vector3 &vecPoint);
	bool simulateFireBullet(EntityWeapon *weap, FireBulletData &data);
	bool handleBulletPenetration(WeapInfo_t *wpn_data, FireBulletData &data);
	bool traceToExit(Vector3 &end, trace_t &tr, float x, float y, float z, float x2, float y2, float z2, trace_t *trace);
	bool traceToExit(Vector3 &end, trace_t *enter_trace, Vector3 start, Vector3 dir, trace_t *exit_trace);
	bool isBreakableEntity(Entity *ent);
	void clipTraceToPlayers(const Vector3 &vecAbsStart, const Vector3 &vecAbsEnd, unsigned int mask, TraceFilter *filter, trace_t *tr);
	bool DidHitNonWorldEntity(Entity *ent);
	void scaleDamage(int hitgroup, Entity *player, float weapon_armor_ratio, float &current_damage);
	bool isArmored(Entity *player, int armorVal, int hitgroup);

	void traceIt(Vector3 &vecAbsStart, Vector3 &vecAbsEnd, unsigned int mask, Entity *ign, trace_t *tr);

	float najFov = 360.0f;
	float maxDistancija = 8192.f;
	float najPretnja = 0.f;
	Vector3 najKost = Vector3(0, 0, 0);
	Entity *najPlyr = nullptr;
	bool bTarget = false;

	UserCMD *userCMD;
	char *bSendPacket;
};

extern std::unique_ptr<AimRage> aimRage;
extern bool lbyUpdated;