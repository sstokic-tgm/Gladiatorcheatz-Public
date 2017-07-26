#pragma once

#include "VT.hpp"

class ConVar
{
public:

	float GetFloat()
	{
		typedef float(__thiscall *o_GetFloat)(void*);
		return VT::vfunc<o_GetFloat>(this, 12)(this);
	}

	int GetInt()
	{
		typedef int(__thiscall *o_GetInt)(void*);
		return VT::vfunc<o_GetInt>(this, 13)(this);
	}

	void SetValue(const char* value)
	{
		typedef void(__thiscall *o_SetValue)(void*, const char*);
		VT::vfunc<o_SetValue>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		typedef void(__thiscall *o_SetValue)(void*, float);
		VT::vfunc<o_SetValue>(this, 15)(this, value);
	}

	void SetValue(int value)
	{
		typedef void(__thiscall *o_SetValue)(void*, int);
		VT::vfunc<o_SetValue>(this, 16)(this, value);
	}

	char unk0[0x4];
	ConVar *pNext;
	int bRegistered;
	char *pszName;
	char *pszHelpString;
	int nFlags;
	char unk1[0x4];
	ConVar *pParent;
	char *pszDefaultValue;
	char *strString;
	int StringLength;
	float fValue;
	int nValue;
	int bHasMin;
	float fMinVal;
	int bHasMax;
	float fMaxVal;
	void *fnChangeCallback;
};

class Console
{
public:

	ConVar *FindVar(const char *var)
	{
		typedef ConVar*(__thiscall *o_FindVar)(void*, const char*);
		return VT::vfunc<o_FindVar>(this, 15)(this, var);
	}

	template<typename ...A>
	void const ConsoleColorPrintf(const Color &c, const char *fmt, const A &...args)
	{
		typedef void(__cdecl *o_ConsoleColorPrintf)(void*, const Color&, const char*, ...);
		VT::vfunc<o_ConsoleColorPrintf>(this, 25)(this, c, fmt, args...);
	}
};

extern Console *p_Console;