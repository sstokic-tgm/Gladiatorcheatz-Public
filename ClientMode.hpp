#pragma once

class ClientMode;

class CHudChat
{

public:

	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		typedef void(__cdecl *o_ChatPrintf)(void*, int, int, const char*,...);
		VT::vfunc<o_ChatPrintf>(this, 26)(this, iPlayerIndex, iFilter, fmt);
	}
};

extern ClientMode *p_ClientMode;
extern CHudChat *p_HudChat;