#include "Gamehooking.hpp"
#include "PredictionSystem.hpp"

void PredictionSystem::start(UserCMD *userCMD)
{
	static bool predictionRandomSeedInit = false;
	if (!predictionRandomSeedInit)
	{
		predictionRandomSeed = *(int**)(csFuncs->findPattern(XorStr("\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04"), XorStr("xx????x????x????xxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0) + 2);
		predictionRandomSeedInit = true;

		//p_Console->ConsoleColorPrintf(Color::LightBlue(), "0x%x", predictionRandomSeed);
	}

	*predictionRandomSeed = MD5_PseudoRandom(userCMD->command_number) & 0x7FFFFFFF;

	oldCurtime = p_GlobVars->curtime;
	oldFrametime = p_GlobVars->frametime;

	p_GlobVars->curtime = Global::locPlayer->getTickBase() * p_GlobVars->interval_per_tick;
	p_GlobVars->frametime = p_GlobVars->interval_per_tick;

	p_GameMovement->StartTrackPredictionErrors(reinterpret_cast<void*>(Global::locPlayer));

	memset(&moveData, 0, sizeof(moveData));
	p_MoveHelper->SetHost(reinterpret_cast<void*>(Global::locPlayer));
	p_Prediction->SetupMove(reinterpret_cast<void*>(Global::locPlayer), userCMD, p_MoveHelper, &moveData);
	p_GameMovement->ProcessMovement(reinterpret_cast<void*>(Global::locPlayer), &moveData);
	p_Prediction->FinishMove(reinterpret_cast<void*>(Global::locPlayer), userCMD, &moveData);
}

void PredictionSystem::end()
{
	p_GameMovement->FinishTrackPredictionErrors(reinterpret_cast<void*>(Global::locPlayer));
	p_MoveHelper->SetHost(0);

	*predictionRandomSeed = -1;

	p_GlobVars->curtime = oldCurtime;
	p_GlobVars->frametime = oldFrametime;
}