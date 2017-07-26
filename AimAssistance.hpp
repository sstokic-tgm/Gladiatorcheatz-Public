#pragma once

class AimAssist
{

public:

	void work(UserCMD *userCMD, char *bSendPacket);

private:

	inline void setTarget(int idx) { iTarget = idx; }
	inline bool hasTarget() { return iTarget != -1; }

	void getBestTarget();
	bool checkTarget(Entity *player);
	bool isBehindSmoke(Vector3 loc, Vector3 rem);
	void targetRegion();

	void rk();
	void trig();

	int iTarget = -1;
	bool bActive = false;

	UserCMD *userCMD;
	char *bSendPacket;
};

extern std::unique_ptr<AimAssist> aimAssist;