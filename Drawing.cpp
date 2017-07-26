#include "Drawing.hpp"

CntDwnTimer coravTimer;

void Drawing::textW(bool center, int font, int x, int y, Color c, wchar_t *pszString)
{
	if (center)
	{
		int wide, tall;
		p_Surface->getTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	p_Surface->drawSetTextColor(c);
	p_Surface->drawSetTextFont(font);
	p_Surface->drawSetTextPos(x, y);
	p_Surface->drawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}

void Drawing::drawString(int font, bool bCenter, int x, int y, Color c, const char *fmt, ...)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	textW(bCenter, font, x, y, c, pszStringWide);

	free(pszStringWide);
}

void Drawing::textWRight(bool right, int font, int x, int y, Color c, wchar_t *pszString)
{
	if (right)
	{
		int wide, tall;
		p_Surface->getTextSize(font, pszString, wide, tall);
		x -= wide;
		//y -= tall;
	}
	p_Surface->drawSetTextColor(c);
	p_Surface->drawSetTextFont(font);
	p_Surface->drawSetTextPos(x, y);
	p_Surface->drawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}

void Drawing::drawStringRight(int font, bool bRight, int x, int y, Color c, const char *fmt, ...)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	textWRight(bRight, font, x, y, c, pszStringWide);

	free(pszStringWide);
}

RECT Drawing::getViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	p_Engine->getScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;

	return Viewport;
}

RECT Drawing::getTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	RECT rect; int x, y;

	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);
	
	p_Surface->getTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;

	return rect;
}

void Drawing::drawOutlinedRect(int x, int y, int w, int h, Color &c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawOutlinedRect(x, y, x + w, y + h);
}

void Drawing::drawFilledRect(int x, int y, int w, int h, Color &c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawFilledRect(x, y, x + w, y + h);
}

void Drawing::drawLine(int x, int y, int x1, int y1, Color c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawLine(x, y, x1, y1);
}

void Drawing::fillRGBA(int x, int y, int w, int h, Color c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawFilledRect(x, y, x + w, y + h);
}

void Drawing::drawHealthBar(int x, int y, float health, int w, int h, Color c)
{
	x -= w / 2;
	y -= h / 2;
	fillRGBA(x, y, w, h + 1, Color(0, 0, 0, 255));
	unsigned int hw = (unsigned int)(((w - 2) * health) / 100);
	fillRGBA(x + 1, y + 1, hw, h - 1, c);
}

void Drawing::drawFlashedBar(int x, int y, float time, int w, int h, Color c)
{
	fillRGBA(x, y, w + 1, h + 1, Color::DWORD2RGBA(0x30000000)/*Color(0, 0, 0, 255)*/);
	float corav = time * 400 / 7.f;
	fillRGBA(x + 1, y + 1, corav - 1, h - 1, c);
}

void Drawing::drawPlyrBoxType(Entity *base, CSFuncs *csFuncs, int type)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	glava.zAechse += 15.0f;

	Vector3 screenOrig, screenGlava;

	if (w2s(glava, screenGlava) && w2s(orig, screenOrig))
	{
		float height = abs(screenOrig.yAechse - screenGlava.yAechse);
		float width = height / 2.0f;

		int leben = player->getLeben();

		crtaj->drawHealthBar(screenGlava.xAechse, screenOrig.yAechse + 5, leben, width, 5, Color((255.f - float(leben) * 2.55f), (float(leben) * 2.55f), 0, 180));
		
		if(type == 1)
			crtaj->drawOutlinedRect(screenGlava.xAechse - width / 2, screenGlava.yAechse, width, height, c);
		else if (type == 2)
		{
			float widthCorner = width / 4;
			float heightCorner = widthCorner;

			crtaj->drawFilledRect(screenGlava.xAechse - width / 2 - (1), screenGlava.yAechse - (1), widthCorner + 2, 3, Color(0, 0, 0, 255));
			crtaj->drawFilledRect(screenGlava.xAechse - width / 2 - (1), screenGlava.yAechse - (1), 3, heightCorner + 2, Color(0, 0, 0, 255));

			crtaj->drawFilledRect((screenGlava.xAechse + width / 2) - widthCorner - (1), screenGlava.yAechse - (1), widthCorner + 2, 3, Color(0, 0, 0, 255));
			crtaj->drawFilledRect(screenGlava.xAechse + width / 2 - (1), screenGlava.yAechse - (1), 3, heightCorner + 2, Color(0, 0, 0, 255));

			crtaj->drawFilledRect(screenGlava.xAechse - width / 2 - (1), screenGlava.yAechse + height - 4, widthCorner + 2, 3, Color(0, 0, 0, 255));
			crtaj->drawFilledRect(screenGlava.xAechse - width / 2 - (1), (screenGlava.yAechse + height) - heightCorner - 4, 3, heightCorner + 2, Color(0, 0, 0, 255));

			crtaj->drawFilledRect((screenGlava.xAechse + width / 2) - widthCorner - (1), screenGlava.yAechse + height - 4, widthCorner + 2, 3, Color(0, 0, 0, 255));
			crtaj->drawFilledRect(screenGlava.xAechse + width / 2 - (1), (screenGlava.yAechse + height) - heightCorner - 4, 3, heightCorner + 3, Color(0, 0, 0, 255));


			crtaj->drawFilledRect(screenGlava.xAechse - width / 2, screenGlava.yAechse, widthCorner, 1, c);
			crtaj->drawFilledRect(screenGlava.xAechse - width / 2, screenGlava.yAechse, 1, heightCorner, c);

			crtaj->drawFilledRect((screenGlava.xAechse + width / 2) - widthCorner, screenGlava.yAechse, widthCorner, 1, c);
			crtaj->drawFilledRect(screenGlava.xAechse + width / 2, screenGlava.yAechse, 1, heightCorner, c);

			crtaj->drawFilledRect(screenGlava.xAechse - width / 2, screenGlava.yAechse + height - 3, widthCorner, 1, c);
			crtaj->drawFilledRect(screenGlava.xAechse - width / 2, (screenGlava.yAechse + height) - heightCorner - 3, 1, heightCorner, c);

			crtaj->drawFilledRect((screenGlava.xAechse + width / 2) - widthCorner, screenGlava.yAechse + height - 3, widthCorner, 1, c);
			crtaj->drawFilledRect(screenGlava.xAechse + width / 2, (screenGlava.yAechse + height) - heightCorner - 3, 1, heightCorner + 1, c);
		}
	}
}

void Drawing::drawPlyrName(int idx, Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	glava.zAechse += 15.0f;

	Vector3 screenOrig, screenGlava;

	if (w2s(glava, screenGlava) && w2s(orig, screenOrig))
	{
		wchar_t plyrName[128];
		swprintf(plyrName, L"%ls", player->getName(idx).c_str());

		crtaj->textW(true, ui_font, screenGlava.xAechse, screenGlava.yAechse - 25, c, plyrName);
	}
}

void Drawing::drawPlyrWeapName(Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	glava.zAechse += 15.0f;

	Vector3 screenOrig, screenGlava;

	if (w2s(glava, screenGlava) && w2s(orig, screenOrig))
	{
		float height = abs(screenOrig.yAechse - screenGlava.yAechse);
		float width = height / 2.0f;

		crtaj->drawString(ui_font, true, screenGlava.xAechse, screenGlava.yAechse + 7, c, csFuncs->getWeapName(player->getWeaponId()));
	}
}

void Drawing::drawPlyrMMinfo(int idx, Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	glava.zAechse += 15.0f;

	Vector3 screenOrig, screenGlava;

	if (w2s(glava, screenGlava) && w2s(orig, screenOrig))
	{
		int leben = player->getLeben();

		char cCompRank[128];
		char cCompWins[128];
		sprintf(cCompRank, "Rank: %s", ranks[player->getCompRank(idx)]);
		sprintf(cCompWins, "Wins: %d", player->getCompWins(idx));

		crtaj->drawString(ui_font, true, screenGlava.xAechse, screenGlava.yAechse - 5, c, cCompWins);
		crtaj->drawString(ui_font, true, screenGlava.xAechse, screenGlava.yAechse - 15, c, cCompRank);
	}
}

void Drawing::drawSnapLine(Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	Vector3 screenOrig;

	int width, height;
	p_Engine->getScreenSize(width, height);

	if (w2s(orig, screenOrig))
	{
		crtaj->drawLine(width / 2, height, screenOrig.xAechse, screenOrig.yAechse, c);
	}
}

void Drawing::drawPlyrFlash(Entity *base, CSFuncs *csFuncs)
{
	Entity *locPlyr = base;

	if (!locPlyr)
		return;

	if (!p_Engine->isIG() || !p_Engine->isConn())
		coravTimer.reset();


	int width, height;
	p_Engine->getScreenSize(width, height);

	float *coravDu = (float*)((DWORD)locPlyr + offs.coravDur);

	if (*coravDu > 0.f)
	{
		if (*coravDu > coravTimer.getRemainingTime())
			coravTimer.start(*coravDu);
		else
			coravTimer.reset();

		*coravDu = 0.f;
	}

	if (coravTimer.getRemainingTime() > 0.0f)
	{
		crtaj->drawFlashedBar((width / 2) - 120, (height / 2) + 300, coravTimer.getRemainingTime(), (coravTimer.getTimeLength() * 400 / 7.f), 14, coravTimer.getRemainingTime() > 2.5f ? Color(255, 0, 0, 255) : Color(0, 255, 0, 255));

		char prozent[101];
		sprintf(prozent, "%.2f%%", coravTimer.getPercentge());

		crtaj->drawString(ui_font, false, ((width / 2) - 120) + ((coravTimer.getTimeLength() * 400 / 7.f) / 2), ((height / 2) + 300), Color::DWORD2RGBA(0xff33C8F3), prozent);
	}
}

void Drawing::drawC4Planted(Entity *base, CSFuncs *csFuncs)
{
	Entity *c4ent = base;

	if (!c4ent || c4ent == NULL)
		return;

	bool c4planted = *(bool*)(c4ent + offs.c4Tick);
	bool c4def = *(bool*)(c4ent + offs.c4Def);
	float c4remTime = *(float*)(c4ent + offs.c4Time);


	bool stopTime = false;

	if (c4planted && c4def != true)
	{
		float timeLeft = (p_GlobVars->curtime - c4remTime) * -1;

		if (timeLeft <= 0.f)
			stopTime = true;
		else
			stopTime = false;

		if (stopTime == false)
		{
			Vector3 orig = c4ent->getViewOriginal();

			Vector3 screenOrig;

			char buf[50];
			sprintf(buf, "%.2f s", timeLeft);

			if (w2s(orig, screenOrig))
			{
				crtaj->drawString(ui_font, true, screenOrig.xAechse, screenOrig.yAechse + 10, Color(255, 0, 0, 255), buf);
				crtaj->fillRGBA(screenOrig.xAechse, screenOrig.yAechse, 4, 4, Color(255, 0, 0, 255));
			}
		}
	}
}

void Drawing::drawAimSpot(Entity *base, int skelId, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 spot = player->getSkeletById(skelId);

	Vector3 screenSpot;

	if (w2s(spot, screenSpot))
	{
		if (player->getTeam() != locPlyr->getTeam())
			crtaj->fillRGBA(screenSpot.xAechse, screenSpot.yAechse, 3, 3, Color(255, 255, 255, 255));
	}
}

void Drawing::drawSnipCrosshair()
{
	int width, height;

	p_Engine->getScreenSize(width, height);

	width /= 2;
	height /= 2;

	crtaj->drawLine(width + 15, height + 15, width + 3, height + 3, Color(255, 0, 0, 255));
	crtaj->drawLine(width - 15, height + 15, width - 3, height + 3, Color(255, 0, 0, 255));
	crtaj->drawLine(width + 15, height - 15, width + 3, height - 3, Color(255, 0, 0, 255));
	crtaj->drawLine(width - 15, height - 15, width - 3, height - 3, Color(255, 0, 0, 255));
	crtaj->fillRGBA(width, height, 1, 1, Color(0, 255, 0, 255));
}

void Drawing::drawC4Droped(Entity *base, CSFuncs *csFuncs)
{
	Entity *c4ent = base;

	if (!c4ent || c4ent == NULL)
		return;

	Vector3 orig = c4ent->getViewOriginal();

	Vector3 screenOrig;

	if (w2s(orig, screenOrig))
	{
		crtaj->drawString(ui_font, true, screenOrig.xAechse, screenOrig.yAechse + 10, Color(255, 0, 0, 255), "C4");
		crtaj->fillRGBA(screenOrig.xAechse, screenOrig.yAechse, 4, 4, Color(255, 0, 0, 255));
	}
}

void Drawing::drawSkelet(Entity * base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	Vector3 locVecAechse = locPlyr->getVecAechse();
	Vector3 locViewOriginal = locPlyr->getViewOriginal();
	Vector3 neu;
	neu = locViewOriginal + locVecAechse;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	Vector3 orig = player->getViewOriginal();
	Vector3 glava = player->getSkeletById(8);

	bool vis = csFuncs->isVis(neu, glava, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	studiohdr_t *studioHdr = p_ModelInfo->GetStudioModel(player->getModel());

	if (studioHdr)
	{
		static matrix3x4_t kostToWorldOut[128];
		if (player->setupBones(kostToWorldOut, 128, 256, 0))
		//if(player->setupBonesEx(player, 256, kostToWorldOut))
		{
			for (int i = 0; i < studioHdr->numbones; i++)
			{
				mstudiobone_t *studioKost = studioHdr->pBone(i);
				
				if (!studioKost || !(studioKost->flags & 256) || studioKost->parent == -1)
					continue;

				Vector3 screenKost1;
				if (!w2s(Vector3(kostToWorldOut[i][0][3], kostToWorldOut[i][1][3], kostToWorldOut[i][2][3]), screenKost1))
					continue;

				Vector3 screenKost2;
				if (!w2s(Vector3(kostToWorldOut[studioKost->parent][0][3], kostToWorldOut[studioKost->parent][1][3], kostToWorldOut[studioKost->parent][2][3]), screenKost2))
					continue;

				crtaj->drawLine((int)screenKost1.xAechse, (int)screenKost1.yAechse, (int)screenKost2.xAechse, (int)screenKost2.yAechse, c);
			}
		}
	}
}

void Drawing::drawBacktrackedSkelet(Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	if (!locPlyr)
		return;

	int idx = player->getIdx();

	LagRecord *m_LagRecords = lagComp->m_LagRecord[idx];
	LagRecord recentLR = m_LagRecords[mVars.historyIdx];

	for (int i = 0; i < 9; i++)
	{
		Vector3 screenSpot;

		if (w2s(m_LagRecords[i].headSpot, screenSpot))
		{
			if (player->getTeam() != locPlyr->getTeam())
				crtaj->fillRGBA(screenSpot.xAechse, screenSpot.yAechse, 3, 3, Color::Green());
		}
	}

	Vector3 screenSpot2;
	if (w2s(recentLR.headSpot, screenSpot2))
	{
		if (player->getTeam() != locPlyr->getTeam())
			crtaj->fillRGBA(screenSpot2.xAechse, screenSpot2.yAechse, 3, 3, Color::Red());
	}
}

void Drawing::drawDlight(Entity *base, CSFuncs *csFuncs)
{
	Entity *player = base;

	bool playerTeam = player->getTeam() == 2;

	dlight_t *dLight = nullptr;
	dLight = p_IVEffects->CL_AllocDlight(player->getIdx());

	if (playerTeam)
	{
		dLight->origin = player->getViewOriginal();
		dLight->radius = 75.f;
		dLight->color.r = 255;
		dLight->color.g = 0;
		dLight->color.b = 0;
		dLight->color.exponent = 10.f;
		dLight->die = p_GlobVars->curtime + 0.05f;
		dLight->decay = dLight->radius / 5.0f;
		dLight->key = player->getIdx();
	}
	else
	{
		dLight->origin = player->getViewOriginal();
		dLight->radius = 75.f;
		dLight->color.r = 0;
		dLight->color.g = 0;
		dLight->color.b = 255;
		dLight->color.exponent = 10.f;
		dLight->die = p_GlobVars->curtime + 0.05f;
		dLight->decay = dLight->radius / 5.0f;
		dLight->key = player->getIdx();
	}
}