#include "harCs.hpp"
#include "GameMovement.hpp"

RebuildGameMovement::RebuildGameMovement(void)
{

}

void RebuildGameMovement::SetAbsOrigin(Entity *player, const Vector3 &vec)
{
	if (player && player->getMoveTip() == MOVETYPE_WALK)
	{
		trace_t pm;
		TracePlayerBBox(vec, vec, MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, pm, player);
		if (pm.startsolid || pm.allsolid || pm.fraction != 1.0f)
		{
			//player will get stuck, lets not?
			return;
		}
	}

	player->getAbsOriginal() = vec;
}

int RebuildGameMovement::ClipVelocity(Vector3 &in, Vector3 &normal, Vector3 &out, float overbounce)
{
	return 0;
}

int RebuildGameMovement::TryPlayerMove(Entity *player, Vector3 *pFirstDest, trace_t *pFirstTrace)
{
	return 0;
}

void RebuildGameMovement::Accelerate(Entity *player, Vector3 &wishdir, float wishspeed, float accel)
{
	
}

void RebuildGameMovement::AirAccelerate(Entity *player, Vector3 &wishdir, float wishspeed, float accel)
{

}

void RebuildGameMovement::AirMove(Entity *player)
{
	
}

void RebuildGameMovement::StepMove(Entity *player, Vector3 &vecDestination, trace_t &trace)
{
	
}

void RebuildGameMovement::TracePlayerBBox(const Vector3 &start, const Vector3 &end, unsigned int fMask, int collisionGroup, trace_t& pm, Entity *player)
{
	Ray_t ray;
	TraceFilter filter;
	filter.pSkip = reinterpret_cast<void*>(player);

	ray.Init(start, end, player->getCollideable()->vecMins(), player->getCollideable()->vecMax());
	p_EngineTrace->TraceRay(ray, fMask, &filter, &pm);
}

void RebuildGameMovement::WalkMove(Entity *player)
{

}

void RebuildGameMovement::FinishGravity(Entity *player)
{
	
}

void RebuildGameMovement::FullWalkMove(Entity *player)
{
	// ~ deleted parts cause pub release

	StartGravity(player);

	// Fricion is handled before we add in any base velocity. That way, if we are on a conveyor, 
	// we don't slow when standing still, relative to the conveyor.
	if (player->getFlags() & FL_ONGROUND)
	{
		player->getVel()[2] = 0.0;
		Friction(player);
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	if (player->getFlags() & FL_ONGROUND)
	{
		WalkMove(player);
	}
	else
	{
		AirMove(player);  // Take into account movement when in air.
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	// Add any remaining gravitational component.
	FinishGravity(player);


	// If we are on ground, no downward velocity.
	if (player->getFlags() & FL_ONGROUND)
	{
		player->getVel()[2] = 0;
	}

	CheckFalling(player);
}

void RebuildGameMovement::Friction(Entity *player)
{
	
}


void RebuildGameMovement::CheckFalling(Entity *player)
{
	
}

const int nanmask = 255 << 23;
#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)
void RebuildGameMovement::CheckVelocity(Entity *player)
{
	
}
void RebuildGameMovement::StartGravity(Entity *player)
{
	
}