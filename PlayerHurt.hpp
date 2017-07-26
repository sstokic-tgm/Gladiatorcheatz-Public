#pragma once

#include "IGameEvent.hpp"

struct HitMarker
{
	float expTime;
	int dmg;
};

class PlayerHurtEvent : public GameEventListener2
{
public:

	void FireGameEvent(GameEvent *gEvent);
	int GetEventDebugID(void);

	void registerSelf(void);
	void unregisterSelf(void);

	void paint(void);

private:

	std::vector<HitMarker> hitMarkers;
};

extern PlayerHurtEvent *playerHurtEvent;