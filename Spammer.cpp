#include "Gamehooking.hpp"
#include "Spammer.hpp"

std::vector<std::string> msgs =
{
	"Gladiatorcheatz owns me and all",
	"Your p2c sucks my sword",
	"Are you not entertained? Are you not entertained? Is this not why you are here?",
	"You would fight me?",
	"Why not? Do you think I am afraid?",
	"I think you've been afraid all your life.",
	"Strength and honor.",
	"The gods have seen it fit yet again to spread cheeks and jam cock in ass.",
	"A Gladiator does not fear death. He embraces it. Caresses it. Fucks it.",
	"Each time he enters the arena, he slips his cock in the mouth of the beast and prays to thrust home before the jaws snap shut.",
	"You squealed like a girl when I nailed you to the cross.",
	"And your wife... moaned like a whore when I ravaged her again and again... and again.",
	"I am required to kill, so I kill."
};

void Spammer::spam()
{
	if (!p_Engine->isIG())
		return;

	long curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timestamp = curTime;

	if ((curTime - timestamp) < 850)
		return;

	if (mVars.bEnableSpam)
	{
		if (msgs.empty())
			return;

		srand(time(NULL));

		std::string msg = msgs[rand() % msgs.size()];

		std::string str;
		str.append("say ");
		str.append(msg);

		p_Engine->execClientCMD(str.c_str());
	}

	timestamp = curTime;
}