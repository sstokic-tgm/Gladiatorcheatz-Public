#include "Gamehooking.hpp"
#include "Misc.hpp"

RevealRanksFn RevealRanks;
IsRdyFn IsRdy;

int promene = -1;

void Misc::bhop(UserCMD *userCMD)
{
	DWORD eng = csFuncs->getEngine();

	if (!Global::locPlayer)
		return;

	bool locPlyrLifeSt = Global::locPlayer->getLifeState();
	int locPlyrCon = csFuncs->getLocPlyrConnect(eng);

	if (Global::locPlayer && !locPlyrLifeSt && locPlyrLifeSt == 0 && locPlyrCon == 6)
	{
		if ((userCMD->buttons & IN_JUMP) && !(Global::locPlayer->getFlags() & FL_ONGROUND))
		{
			userCMD->buttons &= ~IN_JUMP;

			if (mVars.bEnableAS)
			{
				bool backwards = false;
				if (userCMD->mousedx < 0) userCMD->sidemove = backwards ? 650.f : -650.f;
				if (userCMD->mousedx > 0) userCMD->sidemove = backwards ? -650.f : 650.f;
			}
		}
	}
}

void Misc::revealRanks()
{
	static float fArr[3] = { 0.f, 0.f, 0.f };

	RevealRanks = (RevealRanksFn)(offs.revealAllRank);
	RevealRanks(fArr);
}

void Misc::isRdy()
{
	IsRdy = (IsRdyFn)(offs.isRdy);
	IsRdy();

	FLASHWINFO flash;
	flash.cbSize = sizeof(FLASHWINFO);
	flash.hwnd = prozor;
	flash.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	flash.uCount = 0;
	flash.dwTimeout = 0;
	FlashWindowEx(&flash);
}

void Misc::fakeLag(char *bSendPacket)
{
	DWORD eng = csFuncs->getEngine();

	if (!Global::locPlayer)
		return;

	bool locPlyrLifeSt = Global::locPlayer->getLifeState();
	int locPlyrCon = csFuncs->getLocPlyrConnect(eng);

	if (Global::locPlayer && !locPlyrLifeSt && locPlyrLifeSt == 0 && locPlyrCon == 6)
	{
		int tickLimit = realFakeLagChoke[mVars.fakeLagChoke - 1] + realFakeLagSend[mVars.fakeLagSend - 1];

		if (tickLimit >= 15)
			tickLimit = 14;

		if (iFakeLag > tickLimit)
			iFakeLag = 1;
		else
			iFakeLag = iFakeLag + 1;

		if (4 >= iFakeLag)
			*bSendPacket = TRUE;
		else
			*bSendPacket = FALSE;
	}
}

void Misc::nameChanger()
{
	if (!p_Engine->isIG())
		return;

	if (promene == -1)
		return;

	long curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timestamp = curTime;

	if ((curTime - timestamp) < 150)
		return;

	timestamp = curTime;
	++promene;

	if (promene >= 5)
	{
		std::string name = "\n";
		char chars[3] = { '\n', '\0', '\t' };

		for (int i = 0; i < 127; i++)
			name += chars[rand() % 2];

		changeName(name.c_str());

		promene = -1;

		return;
	}

	changeName(setStrRight("Gladiatorcheatz", strlen("Gladiatorcheatz") + promene));
}

void Misc::changeName(const char *name)
{
	ConVar *cv = p_Console->FindVar("name");
	*(int*)((DWORD)&cv->fnChangeCallback + 0xC) = 0;
	cv->SetValue(name);
}

const char *Misc::setStrRight(std::string txt, unsigned int val)
{
	txt.insert(txt.length(), val - txt.length(), ' ');

	return txt.c_str();
}