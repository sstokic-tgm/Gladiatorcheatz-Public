#pragma once


class Entity;
class Vector3;
class MoveData;
class MoveHelper;
class GameMovement;
class Prediction;

enum Collision_Group_t
{
	COLLISION_GROUP_NONE = 0,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
									  // TF2, this filters out other players and CBaseObjects
	COLLISION_GROUP_NPC,			// Generic NPC group
	COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
	COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
	COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
	COLLISION_GROUP_PROJECTILE,		// Projectiles!
	COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
	COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
	COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
	COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

	COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
	COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other

	LAST_SHARED_COLLISION_GROUP
};

class MoveData
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	bool m_bNoAirControl : 1;

	void *m_nPlayerHandle;    // edict index on server, client entity handle on client

	int m_nImpulseCommand;    // Impulse command issued.
	Vector3 m_vecViewAngles;    // Command view angles (local space)
	Vector3 m_vecAbsViewAngles;    // Command view angles (world space)
	int m_nButtons;            // Attack buttons.
	int m_nOldButtons;        // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector3 m_vecVelocity;        // edict::velocity        // Current movement direction.
	Vector3 m_vecOldVelocity;
	float somefloat;
	Vector3 m_vecAngles;        // edict::angles
	Vector3 m_vecOldAngles;

	// Output only
	float m_outStepHeight;    // how much you climbed this move
	Vector3 m_outWishVel;        // This is where you tried 
	Vector3 m_outJumpVel;        // This is your jump velocity
										   // Movement constraints    (radius 0 means no constraint)
	Vector3 m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	bool m_bConstraintPastRadius;        ///< If no, do no constraining past Radius.  If yes, cap them to SpeedFactor past radius

	void SetAbsOrigin(const Vector3 &vec);
	const Vector3 &GetAbsOrigin() const;

private:
	Vector3 m_vecAbsOrigin;        // edict::origin
};

class MoveHelper
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle; // edict index on server, client entity handle on client=
	int m_nImpulseCommand; // Impulse command issued.
	Vector3 m_vecViewAngles; // Command view angles (local space)
	Vector3 m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons; // Attack buttons.
	int m_nOldButtons; // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector3 m_vecVelocity; // edict::velocity // Current movement direction.
	Vector3 m_vecAngles; // edict::angles
	Vector3 m_vecOldAngles;
	float m_outStepHeight; // how much you climbed this move
	Vector3 m_outWishVel; // This is where you tried 
	Vector3 m_outJumpVel; // This is your jump velocity
	Vector3 m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	float m_flUnknown[5];
	Vector3 m_vecAbsOrigin;

	void SetHost(void *host)
	{
		typedef void(__thiscall *o_SetHost)(void*, void*);
		VT::vfunc<o_SetHost>(this, 1)(this, host);
	}
};

class GameMovement
{
public:

	void ProcessMovement(void *player, void *move)
	{
		typedef void(__thiscall *o_ProcessMovement)(void*, void*, void*);
		VT::vfunc<o_ProcessMovement>(this, 1)(this, player, move);
	}

	void StartTrackPredictionErrors(void *player)
	{
		typedef void(__thiscall *o_StartTrackPredictionErrors)(void*, void*);
		VT::vfunc<o_StartTrackPredictionErrors>(this, 3)(this, player);
	}

	void FinishTrackPredictionErrors(void *player)
	{
		typedef void(__thiscall *o_FinishTrackPredictionErrors)(void*, void*);
		VT::vfunc<o_FinishTrackPredictionErrors>(this, 4)(this, player);
	}
};

class Prediction
{
public:

	void SetupMove(void *player, UserCMD *userCMD, MoveHelper *helper, void *move)
	{
		typedef void(__thiscall *o_SetupMove)(void*, void*, UserCMD*, MoveHelper*, void*);
		VT::vfunc<o_SetupMove>(this, 20)(this, player, userCMD, helper, move);
	}

	void FinishMove(void *player, UserCMD *userCMD, void *move)
	{
		typedef void(__thiscall *o_FinishMove)(void*, void*, UserCMD*, void*);
		VT::vfunc<o_FinishMove>(this, 21)(this, player, userCMD, move);
	}
};

class RebuildGameMovement
{
private:

	// ~ deleted cause pub release

private:

public:

	RebuildGameMovement(void);

	void SetAbsOrigin(Entity *entity, const Vector3 &vec);
	void FullWalkMove(Entity *entity);
	void CheckVelocity(Entity *entity);
	void FinishGravity(Entity *entity);
	void StepMove(Entity *entity, Vector3 &vecDestination, trace_t &trace);
	int ClipVelocity(Vector3 &in, Vector3 &normal, Vector3 &out, float overbounce);
	int TryPlayerMove(Entity *entity, Vector3 *pFirstDest, trace_t *pFirstTrace);
	void Accelerate(Entity *entity, Vector3 &wishdir, float wishspeed, float accel);
	void Friction(Entity *entity);
	void AirAccelerate(Entity *entity, Vector3 &wishdir, float wishspeed, float accel);
	void AirMove(Entity *entity);
	void WalkMove(Entity *entity);
	void CheckFalling(Entity *entity);
	void StartGravity(Entity *entity);
	void TracePlayerBBox(const Vector3 &start, const Vector3 &end, unsigned int fMask, int collisionGroup, trace_t &pm, Entity *entity);
};

extern RebuildGameMovement *rebGameMovement;
extern MoveHelper *p_MoveHelper;
extern GameMovement *p_GameMovement;
extern Prediction *p_Prediction;