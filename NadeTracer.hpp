#pragma once

#include "harCs.hpp"
#include "Drawing.hpp"
#include <set>

class Timer
{
private:

	__int64 initialTS, currentTS;
	float secsPerCount;

public:

	Timer()
	{
		__int64 countsPerSec = initialTS = currentTS = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		if (countsPerSec == 0)
			secsPerCount = 1.0f;
		else
			secsPerCount = 1.0f / (float)countsPerSec;
	}

	inline void init()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&initialTS);
	}

	inline float diff()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTS);
		return ((float)currentTS - (float)initialTS)*secsPerCount;
	}
};

struct TraceObj
{
	TraceObj() = default;
	TraceObj(Color, unsigned int);

	Color colorTrace;
	unsigned int maxLength;
	std::vector<Vector3> vecTracePoints;
	Timer timerPointLock;
};

class NadeTracer
{
private:
	Drawing *m_crtaj;

	std::unordered_map<Entity*, TraceObj> mapGrenades;
	std::set<Entity*> setGrenadeExists;

public:
	NadeTracer(Drawing*);
	void addTracer(Entity*, Color, unsigned int);
	void draw();
	void clear();
};