#include "Gamehooking.hpp"
#include "RoundStart.hpp"

void RoundStartEvent::FireGameEvent(GameEvent *gEvent)
{
	if (!gEvent)
		return;

	lagComp->initLagRecord();
}

int RoundStartEvent::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void RoundStartEvent::registerSelf(void)
{
	p_GameEventManager2->AddListener(this, "round_start", false);
}

void RoundStartEvent::unregisterSelf(void)
{
	p_GameEventManager2->RemoveListener(this);
}