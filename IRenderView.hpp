#pragma once

#include "VT.hpp"

class IVRenderView
{

public:

	void SetBlend(float blend)
	{
		typedef void(__thiscall *o_setBlend)(void*, float);
		VT::vfunc<o_setBlend>(this, 4)(this, blend);
	}

	void SetColorModulation(float const *blend)
	{
		typedef void(__thiscall *o_setColModul)(void*, float const*);
		VT::vfunc<o_setColModul>(this, 6)(this, blend);
	}
};

class IRenderView;

extern IRenderView *p_RenderView;
extern IVRenderView *p_IVRenderView;