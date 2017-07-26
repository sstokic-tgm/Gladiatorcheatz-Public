#include "harCs.hpp"
#include "Surface.hpp"

void Surface::drawSetColor(Color col)
{
	typedef void(__thiscall* o_DrawSetColor)(void*, Color);
	return VT::vfunc<o_DrawSetColor>(this, 14)(this, col);
}
void Surface::drawFilledRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* o_DrawSetColor)(void*, int, int, int, int);
	return VT::vfunc<o_DrawSetColor>(this, 16)(this, x0, y0, x1, y1);
}
void Surface::drawOutlinedRect(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* o_DrawOutlinedRect)(void*, int, int, int, int);
	return VT::vfunc<o_DrawOutlinedRect>(this, 18)(this, x0, y0, x1, y1);
}
void Surface::drawLine(int x0, int y0, int x1, int y1)
{
	typedef void(__thiscall* o_DrawLine)(void*, int, int, int, int);
	return VT::vfunc<o_DrawLine>(this, 19)(this, x0, y0, x1, y1);
}
void Surface::drawSetTextFont(unsigned long hFont)
{
	typedef void(__thiscall* o_DrawSetTextFont)(void*, unsigned long);
	return VT::vfunc<o_DrawSetTextFont>(this, 23)(this, hFont);
}
void Surface::drawSetTextColor(Color col)
{
	typedef void(__thiscall* o_DrawSetTextColor)(void*, Color);
	return VT::vfunc<o_DrawSetTextColor>(this, 24)(this, col);
}
void Surface::drawSetTextPos(int x, int y)
{
	typedef void(__thiscall* o_DrawSetTextPos)(void*, int, int);
	return VT::vfunc<o_DrawSetTextPos>(this, 26)(this, x, y);
}
void Surface::drawPrintText(wchar_t *text, int textLen, FontDrawType_t drawType)
{
	typedef void(__thiscall* o_DrawPrintText)(void*, wchar_t*, int, FontDrawType_t);
	return VT::vfunc<o_DrawPrintText>(this, 28)(this, text, textLen, drawType);
}
unsigned long Surface::fontCreate()
{
	typedef unsigned long(__thiscall* o_CreateFont)(void*);
	return VT::vfunc<o_CreateFont>(this, 71)(this);
}
bool Surface::setFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax)
{
	typedef bool(__thiscall* o_CreateFont)(void*, unsigned long, const char *, int, int, int, int, int, int, int);
	return VT::vfunc<o_CreateFont>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
}
bool Surface::getTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
{
	typedef bool(__thiscall* o_GetTextSize)(void*, unsigned long, const wchar_t*, int&, int&);
	return VT::vfunc<o_GetTextSize>(this, 79)(this, font, text, wide, tall);
}