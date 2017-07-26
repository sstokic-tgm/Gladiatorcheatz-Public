#pragma once

#include <Windows.h>

namespace VT
{
	template<typename T> static T vfunc(void *base, int index)
	{
		DWORD *vTabella = *(DWORD**)base;
		return (T)vTabella[index];
	}
}