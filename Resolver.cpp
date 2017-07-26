#include "Gamehooking.hpp"
#include "Resolver.hpp"

std::vector<int64_t> resolvingPlayers = {};
std::vector<std::pair<Entity*, Vector3>> data;

void Resolver::resolve(ClientFrameStage_en stage)
{
	if (!p_Engine->isIG())
		return;

	if (!Global::locPlayer || Global::locPlayer == NULL)
		return;

	if (stage == ClientFrameStage_en::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < p_GlobVars->maxClients; i++)
		{
			Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

			if (!player || player == nullptr)
				continue;

			if (player == Global::locPlayer)
				continue;

			if (player->isStationary())
				continue;

			if (player->isImmune())
				continue;

			if (player->getLeben() <= 0)
				continue;

			if (player->getTeam() == Global::locPlayer->getTeam())
				continue;

			playerinfo_t playerInfo;
			p_Engine->getPlyrInfo(i, &playerInfo);

			if (!mVars.bEnableResolve && std::find(resolvingPlayers.begin(), resolvingPlayers.end(), playerInfo.xuid) == resolvingPlayers.end())
				continue;

			data.push_back(std::pair<Entity*, Vector3>(player, player->getEyeAechse()));

			// ~ deleted parts cause pub release

			if (IsEntityMoving(player))
			{
				player->getEyeAechse().yAechse = player->getLowBodYtarg();
				//p_Console->ConsoleColorPrintf(Color::Blue(), "IsEntityMoving: true\n");
			}
			else
			{
				player->getEyeAechse().yAechse = player->getLowBodYtarg() + 180.f; // nice meme :D
				//p_Console->ConsoleColorPrintf(Color(255, 119, 0, 255), "else: true\n");
			}
		}
	}
	else if (stage == ClientFrameStage_en::FRAME_RENDER_END)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			std::pair<Entity*, Vector3> cur = data.at(i);
			cur.first->getEyeAechse() = cur.second;
		}
		data.clear();
	}
}

void Resolver::fireEventOnClientSide(GameEvent *gEvent)
{
	if (!gEvent)
		return;

	if (strcmp(gEvent->getName(), "player_connect_full") == 0 || strcmp(gEvent->getName(), "cs_game_disconnected") == 0)
	{
		if (gEvent->getInt("userid") && p_Engine->getPlyrForUserId(gEvent->getInt("userid")) != p_Engine->getLocPlyr())
			return;

		resolvingPlayers.clear();
	}
}

bool Resolver::IsEntityMoving(Entity *ent)
{
	return (ent->getVel().length2d() > 0.1f && ent->getFlags() & FL_ONGROUND);
}