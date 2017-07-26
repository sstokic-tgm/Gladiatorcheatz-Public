#include "Gamehooking.hpp"
#include "Drawing.hpp"
#include "PlayerHurt.hpp"

void PlayerHurtEvent::FireGameEvent(GameEvent *gEvent)
{
	if (!gEvent)
		return;

	if (mVars.bEnableHitMarkers)
	{
		if (p_Engine->getPlyrForUserId(gEvent->getInt("attacker")) == p_Engine->getLocPlyr())
			hitMarkers.push_back({p_GlobVars->curtime + 0.8f, gEvent->getInt("dmg_health")});
	}
}

int PlayerHurtEvent::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void PlayerHurtEvent::registerSelf(void)
{
	p_GameEventManager2->AddListener(this, "player_hurt", false);
}

void PlayerHurtEvent::unregisterSelf(void)
{
	p_GameEventManager2->RemoveListener(this);
}

void PlayerHurtEvent::paint(void)
{
	static int width = 0, height = 0;
	if (width == 0 || height == 0)
		p_Engine->getScreenSize(width, height);

	float alpha = 0.f;

	for (size_t i = 0; i < hitMarkers.size(); i++)
	{
		float diff = hitMarkers[i].expTime - p_GlobVars->curtime;

		if (diff < 0.f)
		{
			hitMarkers.erase(hitMarkers.begin() + i);
			continue;
		}

		int dist = 24;

		float ratio = 1.f - (diff / 0.8f);
		alpha = (diff / 0.8f) * 2.f;
		crtaj->drawString(ui_font, false, width / 2 + 6 + ratio * dist / 2, height / 2 + 6 + ratio * dist, Color(255, 255, 0, (int)(alpha*255)), std::to_string(hitMarkers[i].dmg).c_str());
	}

	if (hitMarkers.size() > 0)
	{
		int lineSize = 12;
		crtaj->drawLine(width / 2 - lineSize / 2, height / 2 - lineSize / 2, width / 2 + lineSize / 2, height / 2 + lineSize / 2, Color(255, 255, 255, (int)(alpha * 255)));
		crtaj->drawLine(width / 2 + lineSize / 2, height / 2 - lineSize / 2, width / 2 - lineSize / 2, height / 2 + lineSize / 2, Color(255, 255, 255, (int)(alpha * 255)));
	}
}