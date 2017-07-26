#pragma once

class Misc
{
public:

	void bhop(UserCMD *userCMD);
	void revealRanks();
	void isRdy();
	void fakeLag(char *bSendPacket);
	void nameChanger();
	void changeName(const char *name);

private:

	const char *setStrRight(std::string txt, unsigned int value);

	int iFakeLag = 0;
};

extern int promene;

extern std::unique_ptr<Misc> misc;