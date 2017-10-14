#include "Gamehooking.hpp"
#include "Drawing.hpp"
#include "NadeTracer.hpp"
#include "Config.hpp"

class ConfigFile
{
public:

	ConfigFile(const char *path, const char *name)
	{
		this->path = path;
		this->name = name;
	}

public:

	std::string path, name;
};

Drawing *crtaj = nullptr;
MVars mVars;
std::unique_ptr<AimAssist> aimAssist = std::make_unique<AimAssist>();
std::unique_ptr<AimRage> aimRage = std::make_unique<AimRage>();
std::unique_ptr<Misc> misc = std::make_unique<Misc>();
std::unique_ptr<Resolver> resolver = std::make_unique<Resolver>();

std::unique_ptr<Spammer> spammer = std::make_unique<Spammer>();
std::unique_ptr<Skinchanger> skinchanger = std::make_unique<Skinchanger>();
std::unique_ptr<SkinLocalizer> skinLocalizer = std::make_unique<SkinLocalizer>();
std::unique_ptr<PredictionSystem> predictionSystem = std::make_unique<PredictionSystem>();

RebuildGameMovement *rebGameMovement = nullptr;
LagCompensation *lagComp = nullptr;

unsigned long ui_font = NULL;

static float lastLBYtime = 0.f;
bool lbyUpdated = false;
void lbyShallUpdate();

char *bSendPacket;

std::shared_ptr<NadeTracer> nadeTracer;

bool showMainProzor = true;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


std::vector<ConfigFile> getAllConfInFolder(std::string path);
std::vector<std::string> getAllConf();
void loadConf(std::string path);
void writeConf(std::string path);
void createConfFolder(const char *path);
bool fileExists(std::string &file);


void GUI_Init(IDirect3DDevice9*);
void openMenu();
// tabs
void aimAssistTab();
void visualTab();
void miscTab();
void hvhTab();
void playerListTab();
void skinchangerTab();
void drawConfig();
// prozori
void mainProzor();

void SESPThread(CSFuncs *csFuncs);

void __fastcall Handlers::PaintTraverse_h(void *thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	/*if (mVars.bEnableRage && lstrcmpA(XorStr("HudZoom"), p_Panel->getName(vguiPanel)) == 0)
		return;*/

	o_PaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);

	static DWORD MatSystemTopPanel;

	if (!MatSystemTopPanel)
	{
		const char* szName = p_Panel->getName(vguiPanel);

		if (lstrcmpA(szName, XorStr("MatSystemTopPanel")) == 0)
		{
			MatSystemTopPanel = vguiPanel;

			ui_font = p_Surface->fontCreate();
			p_Surface->setFontGlyphSet(ui_font, XorStr("Tahoma"), 14, 350, 0, 0, FONTFLAG_OUTLINE);

			p_Engine->execClientCMD(XorStr("clear"));
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), " ____    ___                __                __                        __                       __                ___                        __             __      \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "/\\  _`\\ /\\_ \\              /\\ \\  __          /\\ \\__                    /\\ \\                     /\\ \\__            /\\_ \\                      /\\ \\           /\\ \\     \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "\\ \\ \\L\\_\\//\\ \\      __     \\_\\ \\/\\_\\     __  \\ \\ ,_\\   ___   _ __   ___\\ \\ \\___      __     __  \\ \\ ,_\\  ____     \\//\\ \\     ___      __     \\_\\ \\     __   \\_\\ \\    \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), " \\ \\ \\L_L \\ \\ \\   /'__`\\   /'_` \\/\\ \\  /'__`\\ \\ \\ \\/  / __`\\/\\`'__\\/'___\\ \\  _ `\\  /'__`\\ /'__`\\ \\ \\ \\/ /\\_ ,`\\     \\ \\ \\   / __`\\  /'__`\\   /'_` \\  /'__`\\ /'_` \\   \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "  \\ \\ \\/, \\\\_\\ \\_/\\ \\L\\.\\_/\\ \\L\\ \\ \\ \\/\\ \\L\\.\\_\\ \\ \\_/\\ \\L\\ \\ \\ \\//\\ \\__/\\ \\ \\ \\ \\/\\  __//\\ \\L\\.\\_\\ \\ \\_\\/_/  /_     \\_\\ \\_/\\ \\L\\ \\/\\ \\L\\.\\_/\\ \\L\\ \\/\\  __//\\ \\L\\ \\  \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "   \\ \\____//\\____\\ \\__/.\\_\\ \\___,_\\ \\_\\ \\__/.\\_\\\\ \\__\\ \\____/\\ \\_\\\\ \\____\\\\ \\_\\ \\_\\ \\____\\ \\__/.\\_\\\\ \\__\\ /\\____\\    /\\____\\ \\____/\\ \\__/.\\_\\ \\___,_\\ \\____\\ \\___,_\\ \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "    \\/___/ \\/____/\\/__/\\/_/\\/__,_ /\\/_/\\/__/\\/_/ \\/__/\\/___/  \\/_/ \\/____/ \\/_/\\/_/\\/____/\\/__/\\/_/ \\/__/ \\/____/    \\/____/\\/___/  \\/__/\\/_/\\/__,_ /\\/____/\\/__,_ / \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), XorStr("\n"));

			char defFile[MAX_PATH] = { 0 };

			char buf[MAX_PATH];
			GetTempPath(MAX_PATH, buf);

			strcpy_s(defFile, buf);
			strcat_s(defFile, "gladiatorcheatz\\");

			createConfFolder(defFile);

			strcat_s(defFile, _HARC_CONFIG);

			if (fileExists(std::string(defFile)))
			{
				loadConf(defFile);
			}

			p_Engine->execClientCMD(XorStr("version"));
			p_Engine->execClientCMD(XorStr("toggleconsole"));

			nadeTracer = std::make_shared<NadeTracer>(NadeTracer(crtaj));
		}
	}

	if (MatSystemTopPanel == vguiPanel)
	{
		if (p_Engine->isConn() && p_Engine->isIG())
		{
			SESPThread(csFuncs);

			if (mVars.bEnableRankRev && GetAsyncKeyState(VK_TAB))
				misc->revealRanks();
		}
	}
}

int iMaxChokedTicks = 4;
int iChokedTicks = -1;
float nextPrimAtt;
float curTime;
bool canFire = false;
bool oldBullet = false;

bool __stdcall Handlers::CreateMove_h(float smt, UserCMD *userCMD)
{
	Global::locPlayer = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	Global::userCMD = userCMD;

	if (mVars.bEnableRage)
		mVars.bEnableSkelet = false;
	else if (mVars.bEnableSkelet)
		mVars.bEnableRage = false;

	if (userCMD->command_number == 0 || !p_Engine->isIG())
		return false;

	if (!Global::locPlayer || !Global::locPlayer->isAlive())
		return Global::bReturn;

	void *base, *inBase;
	__asm mov base, ebp;
	inBase = (void*)base;
	bSendPacket = *(char**)inBase - 0x1C;

	Vector3 strafeang = userCMD->viewangles;
	Vector3 oldAngles = userCMD->viewangles;

	if (mVars.bEnableBhop)
		misc->bhop(userCMD);

	for (int i = 1; i < p_GlobVars->maxClients; i++)
	{
		Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

		if (!player || player == nullptr)
			continue;

		if (player == Global::locPlayer)
			continue;

		if (player->isStationary())
			continue;

		if (player->isImmune())
			continue;

		if (player->getLeben() <= 0)
			continue;

		if (player->getTeam() == Global::locPlayer->getTeam())
			continue;

		Global::oldSimulTime[i] = player->getSimulTime();

		if (mVars.bEnableLagComp)
		{
			lagComp->fakeLagFix(player, mVars.historyIdx);
		}
	}

	auto weap = Global::locPlayer->getWeapon();
	if (weap)
	{
		canFire = true;

		curTime = p_GlobVars->interval_per_tick * (Global::locPlayer->getTickBase() + 1);
		nextPrimAtt = Global::locPlayer->getWeapon()->getNextPrimAtt();

		if (nextPrimAtt > curTime || oldBullet)
			canFire = false;
		if (!(nextPrimAtt > curTime))
			oldBullet = false;
	}

	if (mVars.bEnableTypeX != 0 && mVars.bEnableTypeY != 0)
	{
		aimRage->fixupAngleDifference(userCMD, bSendPacket);
	}

	Vector3 oldAechse = userCMD->viewangles;
	float oldside = userCMD->sidemove;
	float oldforward = userCMD->forwardmove;
	if ((userCMD->buttons & IN_ATTACK || userCMD->buttons & IN_ATTACK2) && canFire)
		userCMD->viewangles = oldAngles;

	predictionSystem->start(userCMD);
	if (canFire)
	{
		aimAssist->work(userCMD, bSendPacket);

		if (mVars.bEnableRage)
			aimRage->work(userCMD, bSendPacket);
	}
	predictionSystem->end();

	if (mVars.bEnableFakeLag)
		misc->fakeLag(bSendPacket);

	if (mVars.bSilent)
	{
		if (canFire && (userCMD->buttons & IN_ATTACK || userCMD->buttons & IN_ATTACK2))
		{
			*bSendPacket = FALSE;
			iChokedTicks++;
		}
		if (iChokedTicks > iMaxChokedTicks)
			*bSendPacket = TRUE;

		if (*bSendPacket)
		{
			iChokedTicks = 0;
			userCMD->viewangles = oldAechse;
			userCMD->sidemove = oldside;
			userCMD->forwardmove = oldforward;
		}
	}

	if (bSendPacket)
		Global::visAechse = Global::locPlayer->getEyeAechse(); // thirdperson AA, to test out

	aimRage->fixMovement(strafeang, userCMD);

	for (int i = 1; i < p_GlobVars->maxClients; i++)
	{
		Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

		if (!player || player == nullptr)
			continue;

		if (player == Global::locPlayer)
			continue;

		if (player->isStationary())
			continue;

		if (player->isImmune())
			continue;

		if (player->getLeben() <= 0)
			continue;

		if (player->getTeam() == Global::locPlayer->getTeam())
			continue;

		if (mVars.bEnableLagComp)
		{
			lagComp->setCurrentEnt(player);
		}
	}

	return false;
}

void __stdcall Handlers::PlaySound_h(const char *folderIme)
{
	o_PlaySound(p_Surface, folderIme);

	if (!mVars.bEnableAA || p_Engine->isIG()) return;

	if (!strcmp(folderIme, XorStr("!UI/competitive_accept_beep.wav")))
		misc->isRdy();
}

void __fastcall Handlers::RenderView_h(void *thisptr, void*, CViewSetup &vSetup, CViewSetup &hvSetup, int nClearFlags, int whatToDraw)
{
	if (p_Engine->isIG() && csFuncs->getLocalPlayer() != NULL)
	{
		if (mVars.bEnableGlow)
		{
			GlowObjectManager *glowManager = (GlowObjectManager*)offs.glowMngr;

			for (auto i = 0; i < glowManager->size; i++)
			{
				auto glowObject = &glowManager->m_GlowObjectDefinitions[i];

				Entity *ent = glowObject->pEntity;

				if (!glowObject->pEntity || glowObject->IsUnused())
					continue;

				if (ent->isPlayer())
				{
					//Entity *locPlyr = reinterpret_cast<Entity*>(csFuncs->getLocalPlayer());

					Vector3 locVecAechse = Global::locPlayer->getVecAechse();
					Vector3 locViewOrig = Global::locPlayer->getViewOriginal();
					Vector3 newVek;
					newVek = locViewOrig + locVecAechse;

					bool playerTeam = ent->getTeam() == 2;

					Vector3 head = ent->getSkeletById(8);

					bool vis = csFuncs->isVis(newVek, head, ent);
					float r = 0.0f, g = 0.0f, b = 0.0f;

					if (vis)
					{
						if (playerTeam)
							r = 0.9f;
						else
							b = 0.9f;
					}
					else
					{
						if (playerTeam)
						{
							r = 0.9f;
							g = 0.627f;
							b = 0.196f;
						}
						else
						{
							r = 0.196f;
							g = 0.627f;
							b = 0.9f;
						}
					}
					glowObject->m_vGlowColor = Vector3(r, g, b);
					glowObject->m_flGlowAlpha = 1.0f;
					glowObject->m_bRenderWhenOccluded = true;
					glowObject->m_bRenderWhenUnoccluded = false;
				}
				else if(strstr(ent->getClientCls()->getName(), XorStr("CWeapon")) || strcmp(ent->getClientCls()->getName(), XorStr("CDEagle")) == 0 || strcmp(ent->getClientCls()->getName(), XorStr("CAK47")) == 0)
				{
					glowObject->m_vGlowColor = Vector3(Color::Green().getBase());
					glowObject->m_flGlowAlpha = 1.0f;
					glowObject->m_bRenderWhenOccluded = true;
					glowObject->m_bRenderWhenUnoccluded = false;
				}
				else if (strcmp(ent->getClientCls()->getName(), XorStr("CPlantedC4")) == 0 || strcmp(ent->getClientCls()->getName(), XorStr("CC4")) == 0)
				{
					glowObject->m_vGlowColor = Vector3(Color::Red().getBase());
					glowObject->m_flGlowAlpha = 1.0f;
					glowObject->m_bRenderWhenOccluded = true;
					glowObject->m_bRenderWhenUnoccluded = false;
				}
				else if ((strstr(ent->getClientCls()->getName(), XorStr("GrenadeProjectile")) != NULL) || (strcmp(ent->getClientCls()->getName(), XorStr("CMolotovProjectile")) == 0) || (strcmp(ent->getClientCls()->getName(), XorStr("CDecoyProjectile")) == 0))
				{
					glowObject->m_vGlowColor = Vector3(Color::LightBlue().getBase());
					glowObject->m_flGlowAlpha = 1.0f;
					glowObject->m_bRenderWhenOccluded = true;
					glowObject->m_bRenderWhenUnoccluded = false;
				}
			}
		}
	}

	o_RenderView(thisptr, vSetup, hvSetup, nClearFlags, whatToDraw);
}

HRESULT __stdcall Handlers::EndScene_h(IDirect3DDevice9 *pDevice)
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

	if (!d3dinit)
		GUI_Init(pDevice);

	ImGui::GetIO().MouseDrawCursor = mVars.open;

	ImGui_ImplDX9_NewFrame();

	if (mVars.open)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(860, 540));
		mainProzor();
		ImGui::PopStyleVar();
	}

	ImGui::Render();

	return o_EndScene(pDevice);
}

HRESULT __stdcall Handlers::Reset_h(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	if (!d3dinit)
		return o_Reset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = o_Reset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

LRESULT __stdcall Handlers::WndProc_h(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:

		pressedKey[VK_LBUTTON] = true;
		break;

	case WM_LBUTTONUP:

		pressedKey[VK_LBUTTON] = false;
		break;

	case WM_RBUTTONDOWN:

		pressedKey[VK_RBUTTON] = true;
		break;

	case WM_RBUTTONUP:

		pressedKey[VK_RBUTTON] = false;
		break;

	case WM_KEYDOWN:

		pressedKey[wParam] = true;
		break;

	case WM_KEYUP:

		pressedKey[wParam] = false;
		break;
	
	default: break;
	}

	openMenu();

	if (d3dinit && mVars.open && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oldWindowProc, hWnd, uMsg, wParam, lParam);
}

Vector3 oldViewPunch;
Vector3 oldAimPunch;
Vector3 *viewPunch;
Vector3 *aimPunch;

void __stdcall Handlers::FrameStageNotify_h(ClientFrameStage_en stage)
{
	if (p_Engine->isIG())
	{
		if (stage == ClientFrameStage_en::FRAME_NET_UPDATE_START)
		{
			if (Global::locPlayer || Global::locPlayer != NULL)
			{
				for (int i = 0; i < p_GlobVars->maxClients; i++)
				{
					Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

					if (!player || player == nullptr)
						continue;

					if (player == Global::locPlayer)
						continue;

					if (player->getTeam() == Global::locPlayer->getTeam())
						continue;

					// ~ deleted cause pub release
					// but you will want to disable interpolation here, if you want to do it properly
				}
			}
		}
	}

	if (stage == ClientFrameStage_en::FRAME_RENDER_START)
	{
		if (p_Engine->isIG() || Global::locPlayer)
		{
			for (int i = 1; i <= p_GlobVars->maxClients; i++)
			{
				if (i == p_Engine->getLocPlyr())
					continue;

				Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				if (!player || player == nullptr)
					continue;

				*(int*)((DWORD)(player) + offs.iCheckForOccl) = p_GlobVars->framecount;
				*(int*)((DWORD)(player) + 0xA28) = 0;
			}

			if (mVars.bNoVisRec)
			{
				viewPunch = (Vector3*)((DWORD)Global::locPlayer + offs.viewPunch);
				aimPunch = (Vector3*)((DWORD)Global::locPlayer + offs.aimPunch);

				if (aimPunch && viewPunch)
				{
					oldAimPunch = *aimPunch;
					oldViewPunch = *viewPunch;

					*aimPunch = Vector3(0, 0, 0);
					*viewPunch = Vector3(0, 0, 0);
				}
			}
			if (*(bool*)((DWORD)p_Input + 0xA5))
				*(Vector3*)((DWORD)Global::locPlayer + offs.deadFlag4) = Global::visAechse;
		}
	}

	if (stage == ClientFrameStage_en::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		lbyShallUpdate();
	}

	skinchanger->change(stage);
	lagComp->log(stage);
	resolver->resolve(stage);

	o_FrameStageNotify(stage);

	if (p_Engine->isIG())
	{
		if (stage == ClientFrameStage_en::FRAME_RENDER_START)
		{
			if (mVars.bEnableGlow)
			{
				GlowObjectManager *glowManager = (GlowObjectManager*)(offs.glowMngr);

				for (auto i = 0; i < glowManager->size; i++)
				{
					auto glowObject = &glowManager->m_GlowObjectDefinitions[i];

					Entity *ent = glowObject->pEntity;

					if (!glowObject->pEntity || glowObject->IsUnused())
						continue;

					if (ent->isPlayer())
					{
						//Entity *locPlyr = reinterpret_cast<Entity*>(csFuncs->getLocalPlayer());

						if (mVars.bEnableEspIgnTeam)
							if (ent->getTeam() == Global::locPlayer->getTeam())
								continue;

						Vector3 locVecAechse = Global::locPlayer->getVecAechse();
						Vector3 locViewOrig = Global::locPlayer->getViewOriginal();
						Vector3 newVek;
						newVek = locViewOrig + locVecAechse;

						bool playerTeam = ent->getTeam() == 2;

						Vector3 head = ent->getSkeletById(8);

						bool vis = csFuncs->isVis(newVek, head, ent);
						float r = 0.0f, g = 0.0f, b = 0.0f;

						if (vis)
						{
							if (playerTeam)
								r = 0.9f;
							else
								b = 0.9f;
						}
						else
						{
							if (playerTeam)
							{
								r = 0.9f;
								g = 0.627f;
								b = 0.196f;
							}
							else
							{
								r = 0.196f;
								g = 0.627f;
								b = 0.9f;
							}
						}

						glowObject->m_vGlowColor = Vector3(r, g, b);
						glowObject->m_flGlowAlpha = 1.0f;
						glowObject->m_bRenderWhenOccluded = true;
						glowObject->m_bRenderWhenUnoccluded = false;
					}
					else if (strstr(ent->getClientCls()->getName(), XorStr("CWeapon")) || strcmp(ent->getClientCls()->getName(), XorStr("CDEagle")) == 0 || strcmp(ent->getClientCls()->getName(), XorStr("CAK47")) == 0)
					{
						glowObject->m_vGlowColor = Vector3(Color::Green().getBase());
						glowObject->m_flGlowAlpha = 1.0f;
						glowObject->m_bRenderWhenOccluded = true;
						glowObject->m_bRenderWhenUnoccluded = false;
					}
					else if (strcmp(ent->getClientCls()->getName(), XorStr("CPlantedC4")) == 0 || strcmp(ent->getClientCls()->getName(), XorStr("CC4")) == 0)
					{
						glowObject->m_vGlowColor = Vector3(Color::Red().getBase());
						glowObject->m_flGlowAlpha = 1.0f;
						glowObject->m_bRenderWhenOccluded = true;
						glowObject->m_bRenderWhenUnoccluded = false;
					}
					else if ((strstr(ent->getClientCls()->getName(), XorStr("GrenadeProjectile")) != NULL) || (strcmp(ent->getClientCls()->getName(), XorStr("CMolotovProjectile")) == 0) || (strcmp(ent->getClientCls()->getName(), XorStr("CDecoyProjectile")) == 0))
					{
						glowObject->m_vGlowColor = Vector3(Color::LightBlue().getBase());
						glowObject->m_flGlowAlpha = 1.0f;
						glowObject->m_bRenderWhenOccluded = true;
						glowObject->m_bRenderWhenUnoccluded = false;
					}
				}
			}
		}
	}

	if (p_Engine->isIG())
	{
		if (mVars.bNoVisRec && aimPunch && viewPunch)
		{
			*aimPunch = oldAimPunch;
			*viewPunch = oldViewPunch;
		}
	}
}

bool __fastcall Handlers::FireEventClientSide_h(void *thisptr, void*, GameEvent *gEvent)
{
	skinchanger->fireEventOnClientSide(gEvent);
	resolver->fireEventOnClientSide(gEvent);

	return o_FireEventClientSide(thisptr, gEvent);
}

void __fastcall Handlers::BeginFrame_h(void *thisptr, void*, float ft)
{
	misc->nameChanger();
	spammer->spam();

	skinLocalizer->localize();

	o_BeginFrame(thisptr, ft);
}

// helper function to find a word inside a message, used inside DispatchUserMessage_h for the chat bot
bool find_word(const std::string &text, const std::string &word)
{
	size_t pos = 0;

	while ((pos = text.substr(pos).find(word)) != std::string::npos)
	{
		if (!(isalpha(text[pos - 1])) || !(isalpha(text[pos + word.size() + 1])))
			return true;
	}
	return false;
}

std::vector<std::string> blacklisted_words =
{
	"vac inc",
	"ow inc",
	"ow",
	"vac"
};

bool __fastcall Handlers::DispatchUserMessage_h(void *thisptr, void*, unsigned int msg_type, unsigned int unk1, unsigned int nBytes, const void *msg_data)
{
	switch (msg_type)
	{
	case 6:

		if (mVars.bEnableChatBot)
		{
			bf_read data = bf_read(reinterpret_cast<unsigned int>(msg_data));
			data.setOffset(1);

			int ent_idx = data.readByte();
			data.skip(3);
			std::string msg_name = data.readString();
			std::string playerName = data.readString();
			std::string msg = data.readString();

		
			for (size_t i = 0; i < blacklisted_words.size(); i++)
				if (find_word(msg, blacklisted_words.at(i)))
					p_HudChat->ChatPrintf(0, 0, " %sthis system is broken you kid", "\x04");
		}
		/*
		// DEBUG purposes
		std::string location = data.readString();
		std::string unk0 = data.readString();
		bool textallchat = data.readBool();*/

		//p_Console->ConsoleColorPrintf(Color::Purple(), "msg_type: %d bytes: %d\n", msg_type, nBytes);
		//p_Console->ConsoleColorPrintf(Color::LightBlue(), "ent_idx: %d\nmsg_name: %s\nplayerName: %s\nmsg: %s\nlocation: %s\nunk0: %s\ntextallchat: %d\n\n", ent_idx, msg_name.c_str(), playerName.c_str(), msg.c_str(), location.c_str(), unk0.c_str(), textallchat);
	}

	return o_DispatchUserMessage(thisptr, msg_type, unk1, nBytes, msg_data);
}

// not a big fan of DrawModelExecute lel
void __fastcall Handlers::DrawModelExecute_h(void *thisptr, void*, void *ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (pInfo.pModel)
	{
		std::string modelName(p_ModelInfo->GetModelName(pInfo.pModel));

		if (mVars.bEnableWireframe && modelName.find("models/player") != std::string::npos)
		{
			IMaterial *mats[32];

			studiohdr_t *studioHdr = p_ModelInfo->GetStudioModel(pInfo.pModel);
			Entity *ent = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(pInfo.entity_index));

			if (ent)
			{
				p_ModelInfo->GetModelMaterials(pInfo.pModel, studioHdr->numtextures, mats);

				for (int i = 0; i < studioHdr->numtextures; i++)
				{
					IMaterial *mat = mats[i];
					if (!mat)
						continue;

					mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
			}
		}

		if (mVars.bEnableWireframe && modelName.find("arms") != std::string::npos)
		{
			IMaterial *arms = p_MaterialSystem->FindMaterial(modelName.c_str(), TEXTURE_GROUP_MODEL);
			arms->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			p_ModelRender->ForcedMaterialOverride(arms);
		}
	}

	o_DrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	p_ModelRender->ForcedMaterialOverride(NULL);
}

void __fastcall Handlers::TraceRay_h(void *thisptr, void*, const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, trace_t *pTrace)
{
	/*TraceFilterAutowall filter;
	filter.pSkip1 = reinterpret_cast<void*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));
	filter.pSkip2 = reinterpret_cast<void*>(reinterpret_cast<Entity*>(filter.pSkip1)->getWeapon());*/

	o_TraceRay(thisptr, ray, fMask, pTraceFilter, pTrace);

	if(Global::bSearching)
		pTrace->surface.flags |= 4;
}

void Proxies::didSmokeEffect(const PrimitiProxyData *pData, void *pStruct, void *pOut)
{
	if (mVars.bEnableNoSmoke)
		*(bool*)((DWORD)pOut + 0x1) = true;

	o_didSmokeEffect(pData, pStruct, pOut);
}

//-----------------------------------------------------------------------------
// Purpose: Return the difference between two angles
// Input  : destAngle - 
//			srcAngle - 
// Output : float
//-----------------------------------------------------------------------------
float UTIL_AngleDiff(float destAngle, float srcAngle)
{
	float delta;
	delta = destAngle - srcAngle;

	if (destAngle > srcAngle)
	{
		while (delta >= 180)
			delta -= 360;
	}
	else
	{
		while (delta <= -180)
			delta += 360;
	}
	return delta;
}

void lbyShallUpdate()
{
	if (!o_lowerBodyYawTarget)
	{
		netMngr->hookProp("CCSPlayer", "m_flLowerBodyYawTarget", Proxies::lowerBodyYawTarget, o_lowerBodyYawTarget);
		return;
	}

	// ~ deleted cause pub release

	lbyUpdated = false;
}

void Proxies::lowerBodyYawTarget(const PrimitiProxyData *pData, void *pStruct, void *pOut)
{
	// ~ deleted cause pub release
	o_lowerBodyYawTarget(pData, pStruct, pOut);
}

namespace Global
{
	Entity *Global::locPlayer;
	bool bReturn;
	bool bBotting;
	bool bSearching;
	UserCMD *userCMD;
	Vector3 visAechse;
	float oldSimulTime[65];
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}

	class Tab
	{
	private:

		std::vector<std::string> labels;

	public:

		void add(std::string name)
		{
			labels.push_back(name);
		}

		void draw(int *selected)
		{
			ImGuiStyle &style = GetStyle();
			ImVec4 color = style.Colors[ImGuiCol_Button];
			ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
			ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
			ImVec2 max = GetContentRegionMax();
			float size_x = max.x / labels.size() - 20.f;
			float size_y = max.y / labels.size() - 40.f;

			for (size_t i = 0; i < labels.size(); i++)
			{
				if (i == *selected)
				{
					style.Colors[ImGuiCol_Button] = colorActive;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorActive;
				}
				else
				{
					style.Colors[ImGuiCol_Button] = color;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				}

				if (Button(labels.at(i).c_str(), { size_x, size_y }))
					*selected = i;

				/*if (i != labels.size() - 1)
					SameLine();*/
			}

			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}
	};
}

void mainProzor()
{
	static int p = 0;

	ImGui::SetNextWindowSize(ImVec2(860, 540), ImGuiSetCond_FirstUseEver);

	if (ImGui::Begin(XorStr("Gladiatorcheatz"), NULL/*&showMainProzor*/, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
	{
		ImGui::BeginGroup();

		ImGui::Tab tabs;

		tabs.add("Aim Assistance");
		tabs.add("Visual");
		tabs.add("Misc");
		tabs.add("HVH");
		tabs.add("Playerlist");
		tabs.add("Skinchanger");

		tabs.draw(&p);
		
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		drawConfig();
		
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::PushID(p);

		switch (p)
		{
		case 0:

			aimAssistTab();
			break;

		case 1:

			visualTab();
			break;

		case 2:

			miscTab();
			break;

		case 3:

			hvhTab();
			break;

		case 4:

			playerListTab();
			break;

		case 5:

			skinchangerTab();
			break;
		}

		ImGui::PopID();
		ImGui::EndGroup();

		ImGui::End();
	}
}

void aimAssistTab()
{
	ImGui::Columns(2, NULL, false);
	{
		ImGui::BeginChild("AIMASSISTANCE1", ImVec2(0, 0), true);
		{
			ImGui::Text("Aim Assistance");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Aimlock", &mVars.bEnableSkelet);
				ImGui::Checkbox("RCS", &mVars.bEnableRK);
				ImGui::Checkbox("Trigger", &mVars.bEnableTrig);
				ImGui::Checkbox("Trigger With Aimkey", &mVars.bEnableTrigWithKey);
			}
			ImGui::Columns(1);
			ImGui::Separator();

			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Aimkey");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##AIMKEY", &mVars.kAim, opt_Key, 91);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Aimkey 2");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##AIMKEY2", &mVars.kAim2, opt_Key2, 91);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Aim Spot");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Pre Aim Spot");
				ImGui::NewLine();
				ImGui::Text("Bullet After Aim");
				ImGui::NewLine();
				ImGui::Text("After Aim Spot");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##PREAIMSPOT", &mVars.preAim, opt_AimSpot, 4);
				ImGui::NewLine();
				ImGui::SliderInt("##BULLETAFTERAIM", &mVars.afterShots, 3, 15);
				ImGui::NewLine();
				ImGui::Combo("##AFTERAIMSPOT", &mVars.afterAim, opt_AimSpot, 4);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Fov & Smooth");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Fov");
				ImGui::NewLine();
				ImGui::Text("Smooth");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::SliderInt("##FOVLEGIT", &mVars.maxFov, 1, 20);
				ImGui::NewLine();
				ImGui::SliderInt("##SMOOTH", &mVars.umiriFactor, 1, 10);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild("AIMASSISTANCE2", ImVec2(0, 0), true);
		{
			ImGui::Text("Rage");
			ImGui::Separator();

			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Enabled", &mVars.bEnableRage);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Enabling rage, disables Aimlock");
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("General");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Combo("Rage Aimkey", &mVars.cekajTasta, opt_Key, 91);
				ImGui::Checkbox("Use Rage Aimkey", &mVars.bEnableCekaj);
				ImGui::Checkbox("Silent", &mVars.bSilent);
				ImGui::Checkbox("Auto Shoot", &mVars.bEnableAutoPucanje);
				ImGui::Checkbox("Auto Scope", &mVars.bEnableAutoZum);
				ImGui::Checkbox("Auto Crouch", &mVars.bEnableCucati);
				ImGui::Checkbox("Auto Stop", &mVars.bEnableStati);
				ImGui::SliderFloat("Fov", &mVars.hvhFov, 1.f, 360.f, "%.0f");
				ImGui::Checkbox("Lag Compensation", &mVars.bEnableLagComp);
				if (mVars.bEnableLagComp)
					ImGui::SliderInt("History Ticks", &mVars.historyIdx, 0, 10);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Wallbanger");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::SliderFloat("Min. damage", &mVars.wallbangDmg, 0.f, 120.f, "%.2f");
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Hitchance");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::SliderFloat("Amount %", &mVars.hitsansaAmnt, 0.f, 100.0f, "%.2f");
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Target");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Combo("Type", &mVars.bEnableTargetType, opt_TargetType, 3);
				ImGui::Combo("Hitbox", &mVars.hitAim, opt_AimSpot, 4);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Hitscan");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Hitscan Hitbox", &mVars.bHitscanHitbox);
				ImGui::Checkbox("Hitscan", &mVars.bHitscan);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Head", &mVars.bWallbangBones[HITBOX_HEAD]);
				ImGui::Checkbox("Neck", &mVars.bWallbangBones[HITBOX_NECK]);
				ImGui::Checkbox("Pelvis", &mVars.bWallbangBones[HITBOX_PELVIS]);
			}
			ImGui::NextColumn();
			{
				ImGui::Checkbox("Spine", &mVars.bWallbangBones[HITBOX_SPINE]);
				ImGui::Checkbox("Legs", &mVars.bWallbangBones[HITBOX_LEGS]);
				ImGui::Checkbox("Arms", &mVars.bWallbangBones[HITBOX_ARMS]);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void visualTab()
{
	ImGui::Columns(2, NULL, false);
	{
		ImGui::BeginChild("COL1", ImVec2(0, 0), true);
		{
			ImGui::Text("ESP");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("Box Type");
				ImGui::NewLine();
				ImGui::Checkbox("Ignore Team", &mVars.bEnableEspIgnTeam);
				ImGui::Checkbox("Name", &mVars.bEnableEspName);
				ImGui::Checkbox("Weapon Name", &mVars.bEnableEspWeapon);
				ImGui::Checkbox("MM Info", &mVars.bEnableEspMMinfo);
				ImGui::Checkbox("Bomb", &mVars.bEnableEspBomba);
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##BOXTYPE", &mVars.bEnableEspBoxType, opt_EspType, 3);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Draw");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Skelet", &mVars.bEnableDrawSkelet);
				ImGui::Checkbox("Backtracked Skelet", &mVars.bEnableLagCompSkelet);
				ImGui::Checkbox("Snaplines", &mVars.bEnableSnapLine);
				ImGui::Checkbox("Aim Spot", &mVars.bEnableDrawAimSpot);
				ImGui::Checkbox("Nade Tracer", &mVars.bEnableNadeTracer);
				ImGui::Checkbox("Sniper Crosshair", &mVars.bEnableSnipCrosshair);
				ImGui::Checkbox("Glow", &mVars.bEnableGlow);
				ImGui::Checkbox("Dlight", &mVars.bEnableDlight);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}

	ImGui::NextColumn();
	{
		ImGui::BeginChild("COL2", ImVec2(0, 0), true);
		{
			ImGui::Text("Removals");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("No Flash", &mVars.bEnableNoFlash);
				ImGui::Checkbox("No Smoke", &mVars.bEnableNoSmoke);
				ImGui::Checkbox("No Visual Recoil", &mVars.bNoVisRec);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Others");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Hit Marker", &mVars.bEnableHitMarkers);
				ImGui::Checkbox("Wireframe", &mVars.bEnableWireframe);
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void miscTab()
{
	ImGui::Columns(2, NULL, false);
	{
		ImGui::BeginChild("COL1", ImVec2(0, 0), true);
		{
			ImGui::Text("Movement");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("BHOP", &mVars.bEnableBhop);
				ImGui::Checkbox("Auto-Strafe", &mVars.bEnableAS);
			}

			static char nName[127] = "";
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Nickname");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::PushItemWidth(-1);
				ImGui::InputText("##NNAMEINPUT", nName, 127);
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				if (ImGui::Button("Set Nickname"))
					misc->changeName(nName);

				ImGui::SameLine();
				if (ImGui::Button("No Name", ImVec2(-1, 0)))
					promene = 0;
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Other");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Auto-Accept", &mVars.bEnableAA);
				ImGui::Checkbox("Reveal Ranks", &mVars.bEnableRankRev);
				ImGui::Checkbox("Spammer", &mVars.bEnableSpam);
				//ImGui::Checkbox("Chat Bot", &mVars.bEnableChatBot);
				ImGui::Checkbox("Spectator List", &mVars.bEnableSpec);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}

	ImGui::NextColumn();
	{
		ImGui::BeginChild("COL2", ImVec2(0, 0), true);
		{
			ImGui::Text("FakeLag");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Checkbox("Enabled", &mVars.bEnableFakeLag);
				ImGui::Text("Choke");
				ImGui::NewLine();
				ImGui::Text("Send");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::NewLine();
				ImGui::SliderInt("##FAKELAGCHOKE", &mVars.fakeLagChoke, 1, 15);
				ImGui::NewLine();
				ImGui::SliderInt("##FAKELAGSEND", &mVars.fakeLagSend, 1, 15);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void hvhTab()
{
	ImGui::Columns(1, NULL, true);
	{
		ImGui::BeginChild("COL1", ImVec2(0, 0), true);
		{
			ImGui::Text("AntiAim");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				ImGui::Text("AntiAim X");
				ImGui::NewLine();
				ImGui::Text("AntiAim Y");
				ImGui::NewLine();
				ImGui::Text("Fake AntiAim Y");
				ImGui::NewLine();
				ImGui::Checkbox("LBY Breaker", &mVars.bLbyBreaker);
				if(ImGui::IsItemHovered())
					ImGui::SetTooltip("For this to work, you hve to enable AntiAim X & Y");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Combo("##AAX", &mVars.bEnableTypeX, opt_AAxType, 10);
				ImGui::NewLine();
				ImGui::Combo("##AAY", &mVars.bEnableTypeY, opt_AAyType, 9);
				ImGui::NewLine();
				ImGui::Combo("##FAAY", &mVars.bFakeTypeY, opt_AAyType, 9);
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Resolver");
			ImGui::Separator();
			ImGui::Columns(1, NULL, true);
			{
				ImGui::Checkbox("Resolve All", &mVars.bEnableResolve);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Resolve all players");
				/*ImGui::Text("Ticks");
				ImGui::NewLine();
				ImGui::Text("Modulo");*/
			}
			/*ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::NewLine();
				ImGui::SliderInt("##TICKS", &mVars.resTicks, 1, 13);
				ImGui::NewLine();
				ImGui::SliderInt("##MODULO", &mVars.resModulo, 1, 6);
				ImGui::PopItemWidth();
			}*/

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void playerListTab()
{
	ImGui::Columns(1, NULL, true);
	{
		static int currentPlayer = -1;

		if (!p_Engine->isIG() || !reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(currentPlayer))->getGameResConnected(currentPlayer))
			currentPlayer = -1;

		ImGui::ListBoxHeader("##PLAYERS", ImVec2(-1, (ImGui::GetWindowSize().y - 95)));
		if (p_Engine->isIG() && csFuncs->getGameRes())
		{
			ImGui::Columns(5);

			ImGui::Text("ID");
			ImGui::NextColumn();

			ImGui::Text("Nickname");
			ImGui::NextColumn();

			ImGui::Text("Team");
			ImGui::NextColumn();

			ImGui::Text("Rank");
			ImGui::NextColumn();

			ImGui::Text("Wins");
			ImGui::NextColumn();

			std::unordered_map<int, std::vector<int>> players = {
				{ TEAM_UNASSIGNED,{} },
				{ TEAM_SPECTATOR,{} },
				{ TEAM_TERRORIST,{} },
				{ TEAM_COUNTER_TERRORIST,{} },
			};

			for (int i = 0; i < p_GlobVars->maxClients; i++)
			{
				Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				if (i == p_Engine->getLocPlyr())
					continue;

				if (!player->getGameResConnected(i))
					continue;

				players[player->getGameResTeam(i)].push_back(i);
			}

			for (int team = TEAM_UNASSIGNED; team <= TEAM_COUNTER_TERRORIST; team++)
			{
				char *teamName = strdup("");

				switch (team)
				{
				case TEAM_UNASSIGNED:

					teamName = strdup("Unassigned");
					break;

				case TEAM_SPECTATOR:

					teamName = strdup("Spectator");
					break;

				case TEAM_TERRORIST:

					teamName = strdup("Terrorist");
					break;

				case TEAM_COUNTER_TERRORIST:

					teamName = strdup("Counter Terrorist");
					break;
				}

				for (auto it : players[team])
				{
					char id[64];
					sprintf(id, "%d", it);

					playerinfo_t playerInfo;
					p_Engine->getPlyrInfo(it, &playerInfo);

					ImGui::Separator();

					if (ImGui::Selectable(id, it == currentPlayer, ImGuiSelectableFlags_SpanAllColumns))
						currentPlayer = it;

					ImGui::NextColumn();

					ImGui::Text("%s", playerInfo.name/*reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(it))->getName(it).c_str()*/);
					ImGui::NextColumn();

					ImGui::Text("%s", teamName);
					ImGui::NextColumn();

					ImGui::Text("%s", ranks[reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(it))->getCompRank(it)]);
					ImGui::NextColumn();

					ImGui::Text("%d", reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(it))->getCompWins(it));
					ImGui::NextColumn();
				}
			}
		}
		ImGui::ListBoxFooter();

		if (currentPlayer != -1)
		{
			playerinfo_t playerInfo;
			p_Engine->getPlyrInfo(currentPlayer, &playerInfo);

			ImGui::Columns(1);
			{
				bool shouldResolve = std::find(resolvingPlayers.begin(), resolvingPlayers.end(), playerInfo.xuid) != resolvingPlayers.end();
				if (ImGui::Checkbox("Resolve", &shouldResolve))
				{
					if (shouldResolve)
						resolvingPlayers.push_back(playerInfo.xuid);
					else
						resolvingPlayers.erase(std::find(resolvingPlayers.begin(), resolvingPlayers.end(), playerInfo.xuid));
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Add the selected player to the resolver list");
			}
		}
	}
}

void skinchangerTab()
{
	ImGui::BeginChild("SKINCHANGER", ImVec2(0, 0), true);
	{
		static int currWeap = 7;
		static int currWeapSkin = skins[currWeap].pk;
		static float weapW = 0.005f;
		static int weapS = -1;
		static int weapST = -1;
		static char weapName[18];
		static char filterSkins[32];
		static char filterGuns[32];
		static int isCT = 1;

		if (ImGui::Checkbox("Enabled", &mVars.bEnableSkinChange))
			bForceFullUpdate = true;

		ImGui::Separator();

		ImGui::Columns(2);
		ImGui::Text("Guns");
		ImGui::NextColumn();
		ImGui::Text("Skins");
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::Columns(2, NULL, false);
		ImGui::PushItemWidth(-1);
		ImGui::InputText("##FilterGuns", filterGuns, 32);
		ImGui::ListBoxHeader("##GUNS", ImVec2(0, 200));

		for (auto it : guns)
		{
			if (strcmp(it.second, "<-Default->") == 0)
				continue;

			if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
				continue;

			const bool item_selected = (it.first == currWeap);

			ImGui::PushID(it.first);
			if (ImGui::Selectable(it.second, item_selected))
			{
				currWeap = it.first;

				auto keyExist = skins.find(it.first);
				if (keyExist == skins.end())
					currWeapSkin = -1;
				else
					currWeapSkin = skins[it.first].pk;
			}
			ImGui::PopID();
		}

		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		ImGui::InputText("##FilterSkins", filterSkins, 32);
		ImGui::ListBoxHeader("##SKINS", ImVec2(0, 200));
		for (auto it : weapon_skins)
		{
			if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
				continue;

			const bool item_selected = (it.first == currWeapSkin);

			ImGui::PushID(it.first);
			if (ImGui::Selectable(it.second, item_selected))
				currWeapSkin = it.first;

			ImGui::PopID();
		}
		ImGui::ListBoxFooter();
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Columns(2, NULL, true);
		ImGui::Text("Knife");
		ImGui::NextColumn();
		ImGui::Text("Other");
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Columns(3, NULL, false);
		ImGui::SetColumnOffset(1, ImGui::GetWindowWidth() / 2 - 60);
		ImGui::ListBoxHeader("##KNIVES", ImVec2(-1, -1));
		for (auto it : knives)
		{
			const bool item_selected = ((weapon_knife_bayonet + it.first) == currWeap);

			ImGui::PushID(it.first);
			if (ImGui::Selectable(it.second, item_selected))
			{
				currWeap = (weapon_knife_bayonet + it.first);
				currWeapSkin = skins[isCT > 0 ? weapon_knife : weapon_knife_t].pk;
			}
			ImGui::PopID();
		}
		ImGui::ListBoxFooter();
		ImGui::NextColumn();
		ImGui::SetColumnOffset(2, ImGui::GetWindowWidth() / 2 - 4);
		ImGui::RadioButton("CT", &isCT, 1);
		ImGui::RadioButton("T", &isCT, 0);
		ImGui::NextColumn();
		ImGui::BeginChild("Other", ImVec2(-1, -1), true);
		{
			ImGui::InputInt("Skin ID", &currWeapSkin);
			ImGui::SliderFloat("Wear", &weapW, 0.005f, 1.0f);
			ImGui::InputInt("Seed", &weapS);
			ImGui::InputInt("StatTrak", &weapST);
			ImGui::InputText("Name", weapName, 18);
			ImGui::Separator();

			if (ImGui::Button("Apply Skin", ImVec2(-1, 0)))
			{
				if (currWeap >= weapon_knife_bayonet)
				{
					skins[weapon_knife_flip] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_flip.mdl");
					skins[weapon_knife_gut] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_gut.mdl");
					skins[weapon_knife_bayonet] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_bayonet.mdl");
					skins[weapon_knife_m9_bayonet] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_m9_bay.mdl");
					skins[weapon_knife_karambit] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_karam.mdl");
					skins[weapon_knife_tactical] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_tactical.mdl");
					skins[weapon_knife_butterfly] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_butterfly.mdl");
					skins[weapon_knife_survival_bowie] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_survival_bowie.mdl");
					skins[weapon_knife_falchion] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_falchion_advanced.mdl");
					skins[weapon_knife_push] = Skin(-1, -1, -1, -1, -1, "", "models/weapons/v_knife_push.mdl");

					skins[isCT > 0 ? weapon_knife : weapon_knife_t] = Skin
						(
							currWeapSkin == 0 ? -1 : currWeapSkin,
							currWeap,
							weapS,
							weapW,
							weapST,
							weapName,
							""
							);
				}
				else
				{
					skins[currWeap] = Skin
						(
							currWeapSkin == 0 ? -1 : currWeapSkin,
							currWeap,
							weapS,
							weapW,
							weapST,
							weapName,
							""
							);
				}

				bForceFullUpdate = true;
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::EndChild();
	}
}

void drawConfig()
{
	static std::vector<std::string> configItems = getAllConf();
	static int configItemCurrent = -1;

	static char fName[128] = "default";

	ImGui::PushItemWidth(85);
	ImGui::InputText("", fName, 128);
	ImGui::PopItemWidth();

	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		char buf[MAX_PATH];

		GetTempPath(MAX_PATH, buf);

		std::string fPath = std::string(buf) + "gladiatorcheatz\\" + fName + ".glad";
		writeConf(fPath);

		configItems = getAllConf();
		configItemCurrent = -1;
	}

	if (ImGui::Button("Save"))
	{
		if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
		{
			char buf[MAX_PATH];

			GetTempPath(MAX_PATH, buf);

			std::string fPath = std::string(buf) + "gladiatorcheatz\\" + configItems[configItemCurrent] + ".glad";
			writeConf(fPath);
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Remove"))
	{
		if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
		{
			char buf[MAX_PATH];

			GetTempPath(MAX_PATH, buf);

			std::string fPath = std::string(buf) + "gladiatorcheatz\\" + configItems[configItemCurrent] + ".glad";
			std::remove(fPath.c_str());

			configItems = getAllConf();
			configItemCurrent = -1;
		}
	}

	ImGui::PushItemWidth(120);
	if (/*ImGui::ListBox("", &configItemCurrent, configItems, 3)*/ImGui::Combo("", &configItemCurrent, configItems))
	{
		char buf[MAX_PATH];

		GetTempPath(MAX_PATH, buf);

		std::string fPath = std::string(buf) + "gladiatorcheatz\\" + configItems[configItemCurrent] + ".glad";
		loadConf(fPath);
	}
	ImGui::PopItemWidth();

	if (ImGui::Button("Refresh"))
		configItems = getAllConf();
}

void openMenu()
{
	static bool bDown = false;
	static bool bClicked = false;

	if (pressedKey[VK_INSERT])
	{
		bClicked = false;
		bDown = true;
	}
	else if (!pressedKey[VK_INSERT] && bDown)
	{
		bClicked = true;
		bDown = false;
	}
	else
	{
		bClicked = false;
		bDown = false;
	}

	if (bClicked)
	{
		mVars.open = !mVars.open;
		std::string msg = XorStr("cl_mouseenable ") + std::to_string(!mVars.open);
		p_Engine->execClientCMD(msg.c_str());
	}
}

void GUI_Init(IDirect3DDevice9 *pDevice)
{
	static int hue = 140;
	/*static float col_main_sat = 180.f / 255.f;
	static float col_main_val = 161.f / 255.f;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	static float col_back_sat = 59.f / 255.f;
	static float col_back_val = 40.f / 255.f;*/

	ImGui_ImplDX9_Init(prozor, pDevice);

	ImGuiStyle &style = ImGui::GetStyle();

	/*ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
	ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);*/
	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);

	style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 0.85f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, col_area.w + .1f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.85f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.90f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.90f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.90f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	d3dinit = true;
}

void SESPThread(CSFuncs *csFuncs)
{
	if (csFuncs->getLocPlyrConnect(csFuncs->getEngine()) == 6 && p_Engine->isIG() && csFuncs->getLocalPlayer() != NULL)
	{
		Entity *locPlyr = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(p_Engine->getLocPlyr()));

		if (!locPlyr)
			return;

		int locPlyrFrdShots = locPlyr->getFiredShots();

		for (int i = 0; i < 64; i++)
		{
			Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

			if (!player)
				continue;

			if (player == locPlyr)
				continue;

			if (player->isStationary())
				continue;

			if (player->getLeben() <= 0)
				continue;

			if (mVars.bEnableEspIgnTeam)
				if (player->getTeam() == locPlyr->getTeam())
					continue;

			if (mVars.bEnableEspBoxType)
				crtaj->drawPlyrBoxType(player, csFuncs, mVars.bEnableEspBoxType);

			if (mVars.bEnableEspName)
				crtaj->drawPlyrName(i, player, csFuncs);

			if (mVars.bEnableEspWeapon)
				crtaj->drawPlyrWeapName(player, csFuncs);

			if (mVars.bEnableEspMMinfo)
				crtaj->drawPlyrMMinfo(i, player, csFuncs);

			if (mVars.bEnableNoFlash)
				crtaj->drawPlyrFlash(locPlyr, csFuncs);

			if (mVars.bEnableDrawAimSpot)
			{
				if (locPlyrFrdShots > mVars.afterShots || csFuncs->isWeapPistol(locPlyr->getWeaponId()))
					crtaj->drawAimSpot(player, realAimSpot[mVars.afterAim], csFuncs);
				else if (locPlyrFrdShots < mVars.afterShots)
					crtaj->drawAimSpot(player, realAimSpot[mVars.preAim], csFuncs);
			}

			if (mVars.bEnableSnipCrosshair && csFuncs->isWeapSniper(locPlyr->getWeaponId()))
				crtaj->drawSnipCrosshair();

			if (mVars.bEnableSnapLine)
				crtaj->drawSnapLine(player, csFuncs);

			if (mVars.bEnableDrawSkelet)
				crtaj->drawSkelet(player, csFuncs);

			if (mVars.bEnableLagCompSkelet)
				crtaj->drawBacktrackedSkelet(player, csFuncs);

			if (mVars.bEnableDlight)
				crtaj->drawDlight(player, csFuncs);
		}

		// (c) ayyware
		if (mVars.bEnableSpec)
		{
			RECT scrn = crtaj->getViewport();
			int cnt = 0;

			for (int i = 0; i < p_ClientEntList->GetHighestEntityIndex(); i++)
			{
				Entity *player = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				playerinfo_t p_info;

				if (player && player != NULL && player != locPlyr)
				{
					if (p_Engine->getPlyrInfo(i, &p_info) && !player->isAlive() && !player->isStationary())
					{
						DWORD obsTrgt = *(DWORD*)player->getObsTarget();

						if (obsTrgt)
						{
							Entity *trgt = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntityFromHandle(obsTrgt));

							playerinfo_t p_info2;

							if (trgt)
							{
								if (p_Engine->getPlyrInfo(trgt->getIdx(), &p_info2))
								{
									char plyrName[255] = { 0 };
									sprintf_s(plyrName, "%s => %s", p_info.name, p_info2.name);

									RECT txtSize = crtaj->getTextSize(ui_font, plyrName);

									crtaj->drawFilledRect(scrn.right - 260, (scrn.bottom / 2) + (16 * cnt), 260, 16, Color(31, 44, 54, 140/*218*/));
									crtaj->drawString(ui_font, false, scrn.right - txtSize.right - 4, (scrn.bottom / 2) + (16 * cnt), trgt->getIdx() == locPlyr->getIdx() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), plyrName);
									++cnt;
								}
							}
						}
					}
				}
			}
			crtaj->drawOutlinedRect(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * cnt) + 2, Color(9, 82, 128, 255));
			crtaj->drawOutlinedRect(scrn.right - 260, (scrn.bottom / 2), 260, (16 * cnt), Color(90, 90, 90, 160));
		}

		if (mVars.bLbyBreaker)
		{
			RECT scrn = crtaj->getViewport();
			char resultBreaking[255] = { 0 }, angles[255] = { 0 };
			sprintf_s(resultBreaking, "%s", lbyUpdated ? "Safe " : "Critical");
			sprintf_s(angles, "Y: %.3f   LBY: %.3f", Global::locPlayer->getEyeAechse().yAechse, Global::locPlayer->getLowBodYtarg());
			RECT txtSizeBreaking = crtaj->getTextSize(ui_font, resultBreaking);
			RECT txtSizeAngles = crtaj->getTextSize(ui_font, angles);

			crtaj->drawString(ui_font, false, scrn.left + txtSizeBreaking.left - 4, (scrn.bottom / 2) + (16 * 0), lbyUpdated ? Color::Green() : Color::Red(), resultBreaking);
			crtaj->drawString(ui_font, false, scrn.left + txtSizeBreaking.left - 4, (scrn.bottom / 2) + (16 * 1), Color(255, 255, 255, 255), angles);
		}

		if (mVars.bEnableNoSmoke)
			*(int*)offs.smokeCnt = 0;

		if (mVars.bEnableHitMarkers)
			playerHurtEvent->paint();

		if (mVars.bEnableEspBomba)
		{
			for (int i = 0; i < *(DWORD*)(csFuncs->getEntityList() + 0x4); i++)
			{
				Entity *entC4 = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				if (!entC4 || entC4 == NULL)
					continue;

				if (strcmp(entC4->getClientCls()->getName(), XorStr("CPlantedC4")) == 0)
				{
					crtaj->drawC4Planted(entC4, csFuncs);
				}

				/*if(strcmp(entC4->getClientCls()->getName(), "CC4") == 0)
				{
				crtaj->drawC4Droped(entC4, csFuncs);
				}*/
			}
		}

		if (mVars.bEnableNadeTracer)
		{
			for (int i = 0; i < *(DWORD*)(csFuncs->getEntityList() + 0x4); i++)
			{
				Entity *ent = reinterpret_cast<Entity*>(p_ClientEntList->GetClientEntity(i));

				if (!ent || ent == NULL)
					continue;

				if ((strstr(ent->getClientCls()->getName(), XorStr("GrenadeProjectile")) != NULL) || (strcmp(ent->getClientCls()->getName(), XorStr("CMolotovProjectile")) == 0) || (strcmp(ent->getClientCls()->getName(), XorStr("CDecoyProjectile")) == 0))
				{
					nadeTracer->addTracer(ent, Color(0, 255, 0, 255), 100);
					nadeTracer->draw();
				}
			}
			nadeTracer->clear();
		}
	}
}

void loadConf(std::string path)
{
	std::unique_ptr<ConfigMngr::Config> sConfigMgr = std::make_unique<ConfigMngr::Config>();

	if (sConfigMgr->loadInitial(std::string(path)))
	{
		mVars.kAim = sConfigMgr->getInt(XorStr("Aimkey"));
		mVars.kAim2 = sConfigMgr->getInt(XorStr("Aimkey2"));
		mVars.bEnableSkelet = sConfigMgr->getInt(XorStr("Aimlock"));
		mVars.bEnableRage = sConfigMgr->getInt(XorStr("AimRage"));
		mVars.preAim = sConfigMgr->getInt(XorStr("PreAimSpot"));
		mVars.afterShots = sConfigMgr->getInt(XorStr("BulletAfterAim"));
		mVars.afterAim = sConfigMgr->getInt(XorStr("AfterAimSpot"));
		mVars.hitAim = sConfigMgr->getInt(XorStr("Hitbox"));
		mVars.bEnableCekaj = sConfigMgr->getInt(XorStr("UseRageAimkey"));
		mVars.cekajTasta = sConfigMgr->getInt(XorStr("RageAimkey"));
		mVars.bEnableTargetType = sConfigMgr->getInt(XorStr("TargetType"));
		mVars.maxFov = sConfigMgr->getInt(XorStr("Fov"));
		mVars.hvhFov = sConfigMgr->getFloat(XorStr("RageFov"));
		mVars.umiriFactor = sConfigMgr->getInt(XorStr("Smooth"));
		mVars.bEnableRK = sConfigMgr->getInt(XorStr("RCS"));
		mVars.bEnableTrig = sConfigMgr->getInt(XorStr("Trigger"));
		mVars.bEnableTrigWithKey = sConfigMgr->getInt(XorStr("TriggerWithAimkey"));
		mVars.bEnableEspIgnTeam = sConfigMgr->getInt(XorStr("IgnoreTeam"));
		mVars.bEnableEspBoxType = sConfigMgr->getInt(XorStr("ESPBoxType"));
		mVars.bEnableEspName = sConfigMgr->getInt(XorStr("ESPName"));
		mVars.bEnableEspWeapon = sConfigMgr->getInt(XorStr("ESPWeaponName"));
		mVars.bEnableEspMMinfo = sConfigMgr->getInt(XorStr("ESPMMInfo"));
		mVars.bEnableEspBomba = sConfigMgr->getInt(XorStr("ESPBomb"));
		mVars.bEnableDrawSkelet = sConfigMgr->getInt(XorStr("DrawSkelet"));
		mVars.bEnableLagCompSkelet = sConfigMgr->getInt(XorStr("DrawBacktrackedSkelet"));
		mVars.bEnableSnapLine = sConfigMgr->getInt(XorStr("DrawSnaplines"));
		mVars.bEnableDrawAimSpot = sConfigMgr->getInt(XorStr("DrawAimSpot"));
		mVars.bEnableNadeTracer = sConfigMgr->getInt(XorStr("NadeTracer"));
		mVars.bEnableSnipCrosshair = sConfigMgr->getInt(XorStr("SniperCrosshair"));
		mVars.bEnableGlow =sConfigMgr->getInt(XorStr("Glow"));
		mVars.bEnableDlight = sConfigMgr->getInt(XorStr("Dlight"));
		mVars.bEnableNoFlash = sConfigMgr->getInt(XorStr("Flash"));
		mVars.bEnableNoSmoke = sConfigMgr->getInt(XorStr("Smoke"));
		mVars.bNoVisRec = sConfigMgr->getInt(XorStr("VisRecoil"));
		mVars.bEnableHitMarkers = sConfigMgr->getInt(XorStr("HitMarker"));
		mVars.bEnableWireframe = sConfigMgr->getInt(XorStr("Wireframe"));
		mVars.bEnableBhop = sConfigMgr->getInt(XorStr("Bhop"));
		mVars.bEnableAS = sConfigMgr->getInt(XorStr("AutoStrafe"));
		mVars.bEnableAA = sConfigMgr->getInt(XorStr("AutoAccept"));
		mVars.bEnableRankRev = sConfigMgr->getInt(XorStr("RevealRanks"));
		mVars.bEnableTypeX = sConfigMgr->getInt(XorStr("AntiAimX"));
		mVars.bEnableTypeY = sConfigMgr->getInt(XorStr("AntiAimY"));
		mVars.bFakeTypeY = sConfigMgr->getInt(XorStr("FakeAntiAimY"));
		mVars.bEnableFakeLag = sConfigMgr->getInt(XorStr("FakeLag"));
		mVars.fakeLagChoke = sConfigMgr->getInt(XorStr("FakeLagChoke"));
		mVars.fakeLagSend = sConfigMgr->getInt(XorStr("FakeLagSend"));
		mVars.bEnableLagComp = sConfigMgr->getInt(XorStr("LagCompensation"));
		mVars.historyIdx = sConfigMgr->getInt(XorStr("HistoryTicks"));
		mVars.wallbangDmg = sConfigMgr->getFloat(XorStr("WallbangDamage"));
		mVars.hitsansaAmnt = sConfigMgr->getFloat(XorStr("HitchanceAmount"));
		mVars.bHitscanHitbox = sConfigMgr->getInt(XorStr("HitscanHitbox"));
		mVars.bHitscan = sConfigMgr->getInt(XorStr("Hitscan"));
		mVars.bSilent = sConfigMgr->getInt(XorStr("SilentAim"));
		mVars.bEnableCucati = sConfigMgr->getInt(XorStr("AutoCrouch"));
		mVars.bEnableStati = sConfigMgr->getInt(XorStr("AutoStop"));
		mVars.bEnableAutoPucanje = sConfigMgr->getInt(XorStr("AutoShoot"));
		mVars.bEnableAutoZum = sConfigMgr->getInt(XorStr("AutoScope"));
		mVars.bEnableSpam = sConfigMgr->getInt(XorStr("Spammer"));
		//mVars.bEnableChatBot = sConfigMgr->getInt(XorStr("ChatBot"));
		mVars.bLbyBreaker = sConfigMgr->getInt(XorStr("LBYBreaker"));
		mVars.bEnableSpec = sConfigMgr->getInt(XorStr("SpecList"));
		mVars.bEnableResolve = sConfigMgr->getInt(XorStr("ResolveAll"));
		mVars.bEnableSkinChange =sConfigMgr->getInt(XorStr("Skinchanger"));

		p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), XorStr("Config successfully loaded!\n"));
	}
}

void writeConf(std::string path)
{
	std::unique_ptr<Logging::FileLogger> fileLogger = std::make_unique<Logging::FileLogger>(path, path);

	fileLogger->write(XorStr("[Gladiatorcheatz]\n"));

	fileLogger->write(XorStr("Aimkey = %d"), mVars.kAim);
	fileLogger->write(XorStr("Aimkey2 = %d"), mVars.kAim2);
	fileLogger->write(XorStr("Aimlock = %d"), mVars.bEnableSkelet);
	fileLogger->write(XorStr("AimRage = %d"), mVars.bEnableRage);
	fileLogger->write(XorStr("PreAimSpot = %d"), mVars.preAim);
	fileLogger->write(XorStr("BulletAfterAim = %d"), mVars.afterShots);
	fileLogger->write(XorStr("AfterAimSpot = %d"), mVars.afterAim);
	fileLogger->write(XorStr("Hitbox = %d"), mVars.hitAim);
	fileLogger->write(XorStr("UseRageAimkey = %d"), mVars.bEnableCekaj);
	fileLogger->write(XorStr("RageAimkey = %d"), mVars.cekajTasta);
	fileLogger->write(XorStr("TargetType = %d"), mVars.bEnableTargetType);
	fileLogger->write(XorStr("Fov = %d"), mVars.maxFov);
	fileLogger->write(XorStr("RageFov = %f"), mVars.hvhFov);
	fileLogger->write(XorStr("Smooth = %d"), mVars.umiriFactor);
	fileLogger->write(XorStr("RCS = %d"), mVars.bEnableRK);
	fileLogger->write(XorStr("Trigger = %d"), mVars.bEnableTrig);
	fileLogger->write(XorStr("TriggerWithAimkey = %d"), mVars.bEnableTrigWithKey);
	fileLogger->write(XorStr("IgnoreTeam = %d"), mVars.bEnableEspIgnTeam);
	fileLogger->write(XorStr("ESPBoxType = %d"), mVars.bEnableEspBoxType);
	fileLogger->write(XorStr("ESPName = %d"), mVars.bEnableEspName);
	fileLogger->write(XorStr("ESPWeaponName = %d"), mVars.bEnableEspWeapon);
	fileLogger->write(XorStr("ESPMMInfo = %d"), mVars.bEnableEspMMinfo);
	fileLogger->write(XorStr("ESPBomb = %d"), mVars.bEnableEspBomba);
	fileLogger->write(XorStr("DrawSkelet = %d"), mVars.bEnableDrawSkelet);
	fileLogger->write(XorStr("DrawBacktrackedSkelet = %d"), mVars.bEnableLagCompSkelet);
	fileLogger->write(XorStr("DrawSnaplines = %d"), mVars.bEnableSnapLine);
	fileLogger->write(XorStr("DrawAimSpot = %d"), mVars.bEnableDrawAimSpot);
	fileLogger->write(XorStr("NadeTracer = %d"), mVars.bEnableNadeTracer);
	fileLogger->write(XorStr("SniperCrosshair = %d"), mVars.bEnableSnipCrosshair);
	fileLogger->write(XorStr("Glow = %d"), mVars.bEnableGlow);
	fileLogger->write(XorStr("Dlight = %d"), mVars.bEnableDlight);
	fileLogger->write(XorStr("Flash = %d"), mVars.bEnableNoFlash);
	fileLogger->write(XorStr("Smoke = %d"), mVars.bEnableNoSmoke);
	fileLogger->write(XorStr("VisRecoil = %d"), mVars.bNoVisRec);
	fileLogger->write(XorStr("HitMarker = %d"), mVars.bEnableHitMarkers);
	fileLogger->write(XorStr("Wireframe = %d"), mVars.bEnableWireframe);
	fileLogger->write(XorStr("Bhop = %d"), mVars.bEnableBhop);
	fileLogger->write(XorStr("AutoStrafe = %d"), mVars.bEnableAS);
	fileLogger->write(XorStr("AutoAccept = %d"), mVars.bEnableAA);
	fileLogger->write(XorStr("RevealRanks = %d"), mVars.bEnableRankRev);
	fileLogger->write(XorStr("AntiAimX = %d"), mVars.bEnableTypeX);
	fileLogger->write(XorStr("AntiAimY = %d"), mVars.bEnableTypeY);
	fileLogger->write(XorStr("FakeAntiAimY = %d"), mVars.bFakeTypeY);
	fileLogger->write(XorStr("FakeLag = %d"), mVars.bEnableFakeLag);
	fileLogger->write(XorStr("FakeLagChoke = %d"), mVars.fakeLagChoke);
	fileLogger->write(XorStr("FakeLagSend = %d"), mVars.fakeLagSend);
	fileLogger->write(XorStr("LagCompensation = %d"), mVars.bEnableLagComp);
	fileLogger->write(XorStr("HistoryTicks = %d"), mVars.historyIdx);
	fileLogger->write(XorStr("WallbangDamage = %f"), mVars.wallbangDmg);
	fileLogger->write(XorStr("HitchanceAmount = %f"), mVars.hitsansaAmnt);
	fileLogger->write(XorStr("HitscanHitbox = %d"), mVars.bHitscanHitbox);
	fileLogger->write(XorStr("Hitscan = %d"), mVars.bHitscan);
	fileLogger->write(XorStr("SilentAim = %d"), mVars.bSilent);
	fileLogger->write(XorStr("AutoCrouch = %d"), mVars.bEnableCucati);
	fileLogger->write(XorStr("AutoStop = %d"), mVars.bEnableStati);
	fileLogger->write(XorStr("AutoShoot = %d"), mVars.bEnableAutoPucanje);
	fileLogger->write(XorStr("AutoScope = %d"), mVars.bEnableAutoZum);
	fileLogger->write(XorStr("Spammer = %d"), mVars.bEnableSpam);
	//fileLogger->write(XorStr("ChatBot = %d"), mVars.bEnableChatBot);
	fileLogger->write(XorStr("LBYBreaker = %d"), mVars.bLbyBreaker);
	fileLogger->write(XorStr("SpecList = %d"), mVars.bEnableSpec);
	fileLogger->write(XorStr("ResolveAll = %d"), mVars.bEnableResolve);
	fileLogger->write(XorStr("Skinchanger = %d"), mVars.bEnableSkinChange);
}

bool fileExists(std::string &file)
{
	std::ifstream in(file);
	return in.good();
}

void createConfFolder(const char *path)
{
	if (!CreateDirectory(path, NULL)) return;
}

std::vector<ConfigFile> getAllConfInFolder(std::string path)
{
	std::vector<ConfigFile> configs;
	std::string search_path = path + "*.glad";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string fPath = path + fd.cFileName;

				std::string tmp_f_name(fd.cFileName);
				size_t pos = tmp_f_name.find(".");
				std::string fName = (std::string::npos == pos) ? tmp_f_name : tmp_f_name.substr(0, pos);

				ConfigFile newConf(fPath.c_str(), fName.c_str());
				configs.push_back(newConf);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	return configs;
}

std::vector<std::string> getAllConf()
{
	char file[MAX_PATH] = { 0 };
	char buf[MAX_PATH];

	GetTempPath(MAX_PATH, buf);

	strcpy_s(file, buf);
	strcat_s(file, "gladiatorcheatz\\");

	std::vector<ConfigFile> confFiles = getAllConfInFolder(file);
	std::vector<std::string> confs;
	
	for (auto config = confFiles.begin(); config != confFiles.end(); config++)
		confs.push_back(config->name);

	std::sort(confs.begin(), confs.end());

	return confs;
}