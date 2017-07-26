#pragma once

#include "VT.hpp"

class Localize
{
public:

	wchar_t *find(const char *tokenName)
	{
		typedef wchar_t*(__thiscall *o_find)(PVOID, const char*);
		return VT::vfunc<o_find>(this, 11)(this, tokenName);
	}

	const wchar_t *findSafe(const char* tokenName)
	{
		typedef wchar_t*(__thiscall *o_findSafe)(PVOID, const char*);
		return VT::vfunc<o_findSafe>(this, 12)(this, tokenName);
	}
};

extern Localize *p_Localize;