#pragma once

#include "IGameEvent.hpp"

class RoundStartEvent : public GameEventListener2
{
public:

	void FireGameEvent(GameEvent *gEvent);
	int GetEventDebugID(void);

	void registerSelf(void);
	void unregisterSelf(void);
};

extern RoundStartEvent *roundStartEvent;