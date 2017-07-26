#pragma once

#include "harCs.hpp"

class Drawing
{
public:
	void textW(bool center, int font, int x, int y, Color c, wchar_t *pszString);
	void drawString(int font, bool bCenter, int x, int y, Color c, const char *fmt, ...);
	void textWRight(bool right, int font, int x, int y, Color c, wchar_t *pszString);
	void drawStringRight(int font, bool right, int x, int y, Color c, const char *fmt, ...);

	RECT getViewport();
	RECT getTextSize(DWORD font, const char* text);

	void drawOutlinedRect(int x, int y, int w, int h, Color &c);
	void drawFilledRect(int x, int y, int w, int h, Color &c);
	void drawHealthBar(int x, int y, float health, int w, int h, Color c);
	void drawFlashedBar(int x, int y, float time, int w, int h, Color c);
	void drawLine(int x, int y, int x1, int y2, Color c);
	void fillRGBA(int x, int y, int w, int h, Color c);

	void drawPlyrBoxType(Entity *base, CSFuncs *csFuncs, int type);
	void drawPlyrName(int idx, Entity *base, CSFuncs *csFuncs);
	void drawPlyrWeapName(Entity *base, CSFuncs *csFuncs);
	void drawPlyrMMinfo(int idx, Entity *base, CSFuncs *csFuncs);
	void drawPlyrFlash(Entity *base, CSFuncs *csFuncs);
	void drawC4Planted(Entity *base, CSFuncs *csFuncs);
	void drawAimSpot(Entity *base, int skelId, CSFuncs *csFuncs);
	void drawSnipCrosshair();
	void drawC4Droped(Entity *base, CSFuncs *csFuncs);
	void drawSnapLine(Entity *base, CSFuncs *csFuncs);
	void drawSkelet(Entity *base, CSFuncs *csFuncs);
	void drawBacktrackedSkelet(Entity *base, CSFuncs *csFuncs);
	void drawDlight(Entity *base, CSFuncs *csFuncs);
};

extern Drawing *crtaj;

extern unsigned long ui_font;