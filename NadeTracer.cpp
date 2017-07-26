#include "NadeTracer.hpp"

NadeTracer::NadeTracer(Drawing *crtaj)
{
	m_crtaj = crtaj;
}

void NadeTracer::addTracer(Entity* entGrenade, Color colorTrace, unsigned int maxLength)
{
	setGrenadeExists.insert(entGrenade);
	if (mapGrenades.find(entGrenade) == mapGrenades.end())
		mapGrenades[entGrenade] = TraceObj(colorTrace, maxLength);

	if (mapGrenades[entGrenade].timerPointLock.diff() > 0.025f)
	{
		mapGrenades[entGrenade].vecTracePoints.push_back(entGrenade->getViewOriginal());
		if (mapGrenades[entGrenade].vecTracePoints.size() > mapGrenades[entGrenade].maxLength)
			mapGrenades[entGrenade].vecTracePoints.erase(mapGrenades[entGrenade].vecTracePoints.begin());

		mapGrenades[entGrenade].timerPointLock.init();
	}
}

void NadeTracer::draw()
{
	Vector3 vecLastScreenPos;
	bool bInit = false;

	for (auto& traceObj : mapGrenades)
	{
		for (auto& vecPos : traceObj.second.vecTracePoints)
		{
			Vector3 vecScreenPos;
			if (w2s(vecPos, vecScreenPos))
			{
				if (vecScreenPos.zAechse > 0.001f)
				{
					if (bInit)
						m_crtaj->drawLine(vecLastScreenPos.xAechse, vecLastScreenPos.yAechse, vecScreenPos.xAechse, vecScreenPos.yAechse, traceObj.second.colorTrace);

					vecLastScreenPos = vecScreenPos;
					bInit = true;
				}
			}
		}
		bInit = false;
	}
}

void NadeTracer::clear()
{
	for (auto it = mapGrenades.begin(); it != mapGrenades.end(); ++it)
	{
		if (setGrenadeExists.find((*it).first) == setGrenadeExists.end())
		{
			it = mapGrenades.erase(it);
			if (it == mapGrenades.end())
				break;
		}
	}
	setGrenadeExists.clear();
}

TraceObj::TraceObj(Color colorTrace, unsigned int maxLength)
{
	this->colorTrace = colorTrace;
	this->maxLength = maxLength;
	timerPointLock.init();
}