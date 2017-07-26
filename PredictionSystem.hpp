#pragma once

class PredictionSystem
{
public:

	void start(UserCMD *userCMD);
	void end();

private:

	float oldCurtime;
	float oldFrametime;

	MoveData moveData;

	int *predictionRandomSeed;
};

extern std::unique_ptr<PredictionSystem> predictionSystem;