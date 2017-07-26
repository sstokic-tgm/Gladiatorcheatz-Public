#pragma once

#include <deque>

#define TIME_TO_TICKS(dt) ((int)( 0.5f + (float)(dt) / p_GlobVars->interval_per_tick))
#define TICKS_TO_TIME(t) (p_GlobVars->interval_per_tick * (t) )

enum ClientFrameStage_en
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class Resolver {

public:
	void resolve(ClientFrameStage_en stage);
	void fireEventOnClientSide(GameEvent *gEvent);

	bool IsEntityMoving(Entity *ent);
};

inline float RandFloat(float M, float N)
{
	return (float)(M + (rand() / (RAND_MAX / (N - M))));
}

inline float clampY(float in)
{
	float tmp = in;

	while (tmp < -180.0f)
		tmp += 360.0f;
	while (tmp > 180.0f)
		tmp -= 360.0f;

	return tmp;
}

extern std::vector<int64_t> resolvingPlayers;
extern std::unique_ptr<Resolver> resolver;