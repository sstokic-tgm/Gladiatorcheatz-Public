#pragma once

#include "harCs.hpp"
#include "VT.hpp"

class IClientRenderable;

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{
	int flags;
	Vector3 origin;
	float radius;
	ColorRGBExp32 color;
	float die; // stop lighting after this time
	float decay; // drop this each second
	float minLight; // don't add when contributing less
	int key;
	int style; // lightstyle

	// For spotlights. Use m_OuterAngle == 0 for point lights
	Vector3 m_direction; // center of the light cone
	float m_innerAngle;
	float m_outerAngle;

	// If this ptr is set, the dlight will only affect this particular client renderable 
	const IClientRenderable* m_pExclusiveLightReceiver;

	dlight_t() : m_pExclusiveLightReceiver(NULL) {}

	float getRadius() const
	{
		return radius;
	}

	float getRadiusSquared() const
	{
		return radius * radius;
	}

	float isRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};

class IVEffects
{
public:

	dlight_t *CL_AllocDlight(int key)
	{
		typedef dlight_t*(__thiscall *o_CL_AllocDlight)(PVOID, int);
		return VT::vfunc<o_CL_AllocDlight>(this, 4)(this, key);
	}

	dlight_t *CL_AllocElight(int key)
	{
		typedef dlight_t*(__thiscall *o_CL_AllocElight)(PVOID, int);
		return VT::vfunc<o_CL_AllocElight>(this, 5)(this, key);
	}
};

extern IVEffects *p_IVEffects;