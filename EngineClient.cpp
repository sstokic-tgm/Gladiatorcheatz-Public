#include "harCs.hpp"
#include "EngineClient.hpp"

void EngineClient::getScreenSize(int &width, int &height)
{
	typedef void(__thiscall *o_GetScreenSize)(PVOID, int&, int&);
	VT::vfunc<o_GetScreenSize>(this, 5)(this, width, height);
}

const D3DXMATRIX &EngineClient::worldToScreenMatrix()
{
	typedef const D3DXMATRIX &(__thiscall *o_WorldToScreenMatrix)(PVOID);
	return VT::vfunc<o_WorldToScreenMatrix>(this, 37)(this);
}

bool EngineClient::isConn()
{
	typedef bool(__thiscall *o_IsConnected)(PVOID);
	return VT::vfunc<o_IsConnected>(this, 27)(this);
}

bool EngineClient::isIG()
{
	typedef bool(__thiscall *o_IsInGame)(PVOID);
	return VT::vfunc<o_IsInGame>(this, 26)(this);
}

void EngineClient::execClientCMD(const char *cmd)
{
	typedef void(__thiscall *o_ExecClientCMD)(PVOID, const char*);
	VT::vfunc<o_ExecClientCMD>(this, 108)(this, cmd);
}

void EngineClient::execClientCMD_Unrestricted(const char *cmd, const char *flag)
{
	typedef void(__thiscall *o_ExecClientCMD_Unrestricted)(PVOID, const char*, const char*);
	VT::vfunc<o_ExecClientCMD_Unrestricted>(this, 114)(this, cmd, flag);
}

void EngineClient::getViewAechse(Vector3 &aechse)
{
	typedef void(__thiscall *o_GetViewAechse)(PVOID, Vector3&);
	VT::vfunc<o_GetViewAechse>(this, 18)(this, aechse);
}

void EngineClient::setViewAechse(Vector3 &aechse)
{
	typedef void(__thiscall *o_SetViewAechse)(PVOID, Vector3&);
	VT::vfunc<o_SetViewAechse>(this, 19)(this, aechse);
}

bool EngineClient::getPlyrInfo(int idx, playerinfo_t *playerinfo)
{
	typedef bool(__thiscall *o_GetPlyrInfo)(PVOID, int, playerinfo_t *);
	return VT::vfunc<o_GetPlyrInfo>(this, 8)(this, idx, playerinfo);
}

int EngineClient::getPlyrForUserId(int userId)
{
	typedef int(__thiscall *o_getPlyrForUserId)(PVOID, int);
	return VT::vfunc<o_getPlyrForUserId>(this, 9)(this, userId);
}

int EngineClient::getLocPlyr()
{
	typedef int(__thiscall *o_getLocPlyr)(PVOID);
	return VT::vfunc<o_getLocPlyr>(this, 12)(this);
}

INetChannelInfo *EngineClient::getNetChannelInfo()
{
	typedef INetChannelInfo*(__thiscall *o_getNetChannel)(PVOID);
	return VT::vfunc<o_getNetChannel>(this, 78)(this);
}

float EngineClient::getLastTimeStamp()
{
	typedef float(__thiscall *o_getLastTimeStamp)(PVOID);
	return VT::vfunc<o_getLastTimeStamp>(this, 14)(this);
}