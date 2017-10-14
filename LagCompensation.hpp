#pragma once

#include <vector>

class Entity;
class Vector3;

struct LagRecord
{
	Vector3 m_vecOrigin;
	Vector3 m_angEyeAngles;
	float m_fSimulationTime;
	float m_flPoseParameter[24];
	float m_flLowerBodyYawTarget;
	unsigned char flags;
	bool m_bIsBreakingLagComp;
	Vector3 headSpot;

	LagRecord()
	{
		m_vecOrigin.clearVekt();
		m_angEyeAngles.clearVekt();
		m_fSimulationTime = -1.f;
		m_flLowerBodyYawTarget = 0.f;
		flags = -1;
		m_bIsBreakingLagComp = false;
		headSpot.clearVekt();
	}
};

class LagCompensation
{
public:

	LagRecord m_LagRecord[64][11] = {};
	LagRecord m_PrevRecords[64] = {};

public:

	LagCompensation();

	void logEntity(Entity *player);
	void logCurrentEnt(Entity *player);
	void setEntity(Entity *player, LagRecord record);
	void setCurrentEnt(Entity *player);
	void fakeLagFix(Entity *player, int historyIdx);
	void initLagRecord();
	int fixTickcount(Entity *player);
	bool isValidTick(float simTime);
	void log(ClientFrameStage_en stage);
	bool lbyShallUpdate(Entity *player);

private:

	template<class T, class U>
	T clamp(T in, U low, U high);
	float lerpTime();
};

extern LagCompensation *lagComp;