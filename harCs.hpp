#pragma once

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <memory>
#include <random>

#include "XorStr.hpp"
#include "Logger.hpp"

#include "NetMngr.hpp"
#include "VT.hpp"

#include "Offsets.hpp"
#include "MVars.hpp"

#include "Color.hpp"
#include "Vector3.hpp"
#include "VMatrix.hpp"

#include "ChecksumMD5.hpp"
#include "bitbuf.h"

#include "ClientMode.hpp"
#include "ClientEntList.hpp"
#include "EngineClient.hpp"
#include "GlobVars.hpp"
#include "Input.hpp"
#include "Panel.hpp"
#include "Surface.hpp"
#include "EngineTrace.hpp"
#include "Console.hpp"
#include "IVEffects.hpp"
#include "IVModelInfo.hpp"
#include "IRenderView.hpp"
#include "IGameEvent.hpp"
#include "ViewSetup.hpp"
#include "VPhysics.hpp"
#include "IMaterialSystem.hpp"
#include "IVModelRender.hpp"
#include "ILocalize.hpp"
#include "GameMovement.hpp"

#include "Playerinfo.hpp"
#include "Entity.hpp"
#include "CSFuncs.hpp"
#include "PredictionSystem.hpp"
#include "GlowObjectDef.hpp"
#include "AimAssistance.hpp"
#include "AimRage.hpp"
#include "Misc.hpp"
#include "RoundStart.hpp"
#include "PlayerHurt.hpp"
#include "Resolver.hpp"
#include "LagCompensation.hpp"
#include "Spammer.hpp"
#include "Skins.hpp"
#include "Skinchanger.hpp"

#define WIN32_LEAN_AND_MEAN

#ifndef _HARC_CONFIG
#define _HARC_CONFIG  XorStr("harCs.glad")
#endif

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "psapi.lib")

#include "imgui\imgui.h"
#include "imgui\DX9\imgui_impl_dx9.h"

namespace Global
{
	extern Entity *locPlayer;
	extern bool bReturn;
	extern bool bBotting;
	extern bool bSearching;
	extern UserCMD *userCMD;
	extern Vector3 visAechse;
	extern float oldSimulTime[65];
}

double inline __declspec (naked) __fastcall FastSqrt(double n)
{
	_asm fld qword ptr[esp + 4]
	_asm fsqrt
	_asm ret 8
}

void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

static bool screenTransform(const Vector3 &dot, Vector3 &screen)
{
	const D3DXMATRIX w2sM = p_Engine->worldToScreenMatrix();

	screen.xAechse = w2sM.m[0][0] * dot.xAechse + w2sM.m[0][1] * dot.yAechse + w2sM.m[0][2] * dot.zAechse + w2sM.m[0][3];
	screen.yAechse = w2sM.m[1][0] * dot.xAechse + w2sM.m[1][1] * dot.yAechse + w2sM.m[1][2] * dot.zAechse + w2sM.m[1][3];
	screen.zAechse = w2sM.m[2][0] * dot.xAechse + w2sM.m[2][1] * dot.yAechse + w2sM.m[2][2] * dot.zAechse + w2sM.m[2][3];

	float w = w2sM.m[3][0] * dot.xAechse + w2sM.m[3][1] * dot.yAechse + w2sM.m[3][2] * dot.zAechse + w2sM.m[3][3];

	if (screen.zAechse < 0.1f)
		return false;

	if (w < 0.001f) {
		screen.xAechse *= 100000;
		screen.yAechse *= 100000;
		return true;
	}

	float invW = 1.0f / w;
	screen.xAechse *= invW;
	screen.yAechse *= invW;

	return false;
}

static bool w2s(const Vector3 &orig, Vector3 &screen)
{
	if (!screenTransform(orig, screen))
	{
		int width, height;

		p_Engine->getScreenSize(width, height);

		screen.xAechse = (width / 2.0f) + (screen.xAechse * width) / 2;
		screen.yAechse = (height / 2.0f) - (screen.yAechse * height) / 2;

		return true;
	}
	return false;
}

static float DotProduct(const Vector3 &a, const Vector3 &b)
{
	return(a.xAechse*b.xAechse + a.yAechse*b.yAechse + a.zAechse*b.zAechse);
}

static Vector3 CrossProduct(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.yAechse*b.zAechse - a.zAechse*b.yAechse, a.zAechse*b.xAechse - a.xAechse*b.zAechse, a.xAechse*b.yAechse - a.yAechse*b.xAechse);
}

static void VectorTransform(Vector3 &in1, matrix3x4a_t &in2, Vector3 &out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

static const char *ranks[] =
{
	"-",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(csFuncs->findPattern(XorStr("\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\x7D\xF8\x00"), XorStr("x????x????xxxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(csFuncs->findPattern(XorStr("\x55\x8B\xEC\x53\x8B\x5D\x08\x56\x57\x8B\xF9\x33\xF6\x39\x77\x28"), XorStr("xxxxxxxxxxxxxxxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0));
	return (T*)find_hud_element(pThis, name);
}

enum TeamID
{
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

enum CSGO_Weapon_ID
{
	weapon_none = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg553 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516
};