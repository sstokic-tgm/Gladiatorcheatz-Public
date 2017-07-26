#pragma once

extern std::vector<std::pair<int, const char*>> guns;
extern std::vector<std::pair<int, const char*>> knives;
extern std::vector<std::pair<int, const char*>> weapon_skins;

class SkinLocalizer
{
public:

	void localize();

private:

	void localizeGuns();
	void localizeKnives();
};

extern std::unique_ptr<SkinLocalizer> skinLocalizer;