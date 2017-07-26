#include <Windows.h>
#include "Install.hpp"
#include "Gamehooking.hpp"

std::unique_ptr<VFTableHook> g_pPaintTraverseHook = nullptr;
std::unique_ptr<VFTableHook> g_pCreateMoveHook = nullptr;
std::unique_ptr<VFTableHook> g_pPlaySoundHook = nullptr;
std::unique_ptr<VFTableHook> g_pRenderViewHook = nullptr;
std::unique_ptr<VFTableHook> g_pD3DDevHook = nullptr;
std::unique_ptr<VFTableHook> g_pClientHook = nullptr;
std::unique_ptr<VFTableHook> g_pGameEventManagerHook = nullptr;
std::unique_ptr<VFTableHook> g_pMaterialSystemHook = nullptr;
std::unique_ptr<VFTableHook> g_pDMEHook = nullptr;
std::unique_ptr<VFTableHook> g_pTraceRayHook = nullptr;

PaintTraverse_t o_PaintTraverse = nullptr;
CreateMove_t o_CreateMove = nullptr;
PlaySound_t o_PlaySound = nullptr;
RenderView_t o_RenderView = nullptr;
EndScene_t o_EndScene = nullptr;
Reset_t o_Reset = nullptr;
FrameStageNotify_t o_FrameStageNotify = nullptr;
FireEventClientSide_t o_FireEventClientSide = nullptr;
BeginFrame_t o_BeginFrame = nullptr;
DispatchUserMessage_t o_DispatchUserMessage = nullptr;
DrawModelExecute_t o_DrawModelExecute = nullptr;
TraceRay_t o_TraceRay = nullptr;

PrimitiVarProxyFn o_didSmokeEffect = NULL;
PrimitiVarProxyFn o_viewModelSequence = NULL;
PrimitiVarProxyFn o_lowerBodyYawTarget = NULL;

BaseClientDll *p_Client = nullptr;
ClientMode *p_ClientMode = nullptr;
CHudChat *p_HudChat = nullptr;
ClientEntList *p_ClientEntList = nullptr;
EngineClient *p_Engine = nullptr;
Panel *p_Panel = nullptr;
Surface *p_Surface = nullptr;
EngineTrace *p_EngineTrace = nullptr;
Console *p_Console = nullptr;
GlobVars *p_GlobVars = nullptr;
Input *p_Input = nullptr;
IVEffects *p_IVEffects = nullptr;
IVModelInfo *p_ModelInfo = nullptr;
IRenderView *p_RenderView = nullptr;
IVRenderView *p_IVRenderView = nullptr;
GameEventManager2 *p_GameEventManager2 = nullptr;
IPhysicsSurfaceProps *p_PhysSurfProps = nullptr;
IMaterialSystem *p_MaterialSystem = nullptr;
Localize *p_Localize = nullptr;
MoveHelper *p_MoveHelper = nullptr;
GameMovement *p_GameMovement = nullptr;
Prediction *p_Prediction = nullptr;
IVModelRender *p_ModelRender = nullptr;

CSFuncs *csFuncs = nullptr;
RoundStartEvent *roundStartEvent = nullptr;
PlayerHurtEvent *playerHurtEvent = nullptr;

HWND prozor = nullptr;
WNDPROC oldWindowProc = nullptr;

bool d3dinit = false;

bool pressedKey[256] = {};

void init()
{
	while (!GetModuleHandleA(XorStr("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while (!GetModuleHandleA(XorStr("client.dll")) || !GetModuleHandleA(XorStr("engine.dll")) || !GetModuleHandleA(XorStr("vstdlib.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while (!(prozor = FindWindowA(XorStr("Valve001"), NULL))) std::this_thread::sleep_for(std::chrono::milliseconds(200));

	p_Client = Iface::IfaceMngr::getIface<BaseClientDll>(XorStr("client.dll"), XorStr("VClient"));
	p_ClientEntList = Iface::IfaceMngr::getIface<ClientEntList>(XorStr("client.dll"), XorStr("VClientEntityList"));
	p_Engine = Iface::IfaceMngr::getIface<EngineClient>(XorStr("engine.dll"), XorStr("VEngineClient"));
	p_Panel = Iface::IfaceMngr::getIface<Panel>(XorStr("vgui2.dll"), XorStr("VGUI_Panel"));
	p_Surface = Iface::IfaceMngr::getIface<Surface>(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface"));
	p_EngineTrace = Iface::IfaceMngr::getIface<EngineTrace>(XorStr("engine.dll"), XorStr("EngineTraceClient"));
	p_Console = Iface::IfaceMngr::getIface<Console>(XorStr("vstdlib.dll"), XorStr("VEngineCvar"));
	p_IVEffects = Iface::IfaceMngr::getIface<IVEffects>(XorStr("engine.dll"), XorStr("VEngineEffects"));
	p_ModelInfo = Iface::IfaceMngr::getIface<IVModelInfo>(XorStr("engine.dll"), XorStr("VModelInfoClient"));
	p_GameEventManager2 = Iface::IfaceMngr::getIface<GameEventManager2>(XorStr("engine.dll"), XorStr("GAMEEVENTSMANAGER002"), true);
	p_PhysSurfProps = Iface::IfaceMngr::getIface<IPhysicsSurfaceProps>(XorStr("vphysics.dll"), XorStr("VPhysicsSurfaceProps"));
	p_MaterialSystem = Iface::IfaceMngr::getIface<IMaterialSystem>(XorStr("materialsystem.dll"), XorStr("VMaterialSystem"));
	p_Localize = Iface::IfaceMngr::getIface<Localize>(XorStr("localize.dll"), XorStr("Localize_"));
	p_GameMovement = Iface::IfaceMngr::getIface<GameMovement>(XorStr("client.dll"), XorStr("GameMovement"));
	p_Prediction = Iface::IfaceMngr::getIface<Prediction>(XorStr("client.dll"), XorStr("VClientPrediction"));
	p_ModelRender = Iface::IfaceMngr::getIface<IVModelRender>(XorStr("engine.dll"), XorStr("VEngineModel"));
	p_IVRenderView = Iface::IfaceMngr::getIface<IVRenderView>(XorStr("engine.dll"), XorStr("VEngineRenderView0"));

	csFuncs = new CSFuncs;
	csFuncs->init();

	auto d3d = csFuncs->getD3D();

	p_GlobVars = **(GlobVars***)((*(DWORD**)(p_Client))[0] + 0x1B);
	p_Input = *(Input**)((*(DWORD**)p_Client)[15] + 0x1);
	p_ClientMode = **(ClientMode***)((*(DWORD**)p_Client)[10] + 0x5);

	p_HudChat = FindHudElement<CHudChat>("CHudChat"); // (c) Beakers

	// this fuck is crashing on some PCs
	/*do
	{
		p_RenderView = **reinterpret_cast<IRenderView***>(csFuncs->getRenderView());
	} while (!p_RenderView && p_RenderView == NULL);*/

	do
	{
		p_MoveHelper = **(MoveHelper***)(csFuncs->findPattern(XorStr("\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x68"), XorStr("xx????xxxx"), csFuncs->dClientDll, csFuncs->dClientDll + csFuncs->dClientDllSize, 0) + 2);
	} while (!p_MoveHelper && p_MoveHelper == NULL);
	
	if (prozor)
		oldWindowProc = (WNDPROC)SetWindowLongPtr(prozor, GWLP_WNDPROC, (LONG_PTR)Handlers::WndProc_h);

	g_pPaintTraverseHook = std::make_unique<VFTableHook>((PPDWORD)p_Panel, true);
	g_pCreateMoveHook = std::make_unique<VFTableHook>((PPDWORD)p_ClientMode, true);
	g_pPlaySoundHook = std::make_unique<VFTableHook>((PPDWORD)p_Surface, true);
	//g_pRenderViewHook = std::make_unique<VFTableHook>((PPDWORD)p_RenderView, true);
	g_pD3DDevHook = std::make_unique<VFTableHook>((PPDWORD)d3d, true);
	g_pClientHook = std::make_unique<VFTableHook>((PPDWORD)p_Client, true);
	g_pGameEventManagerHook = std::make_unique<VFTableHook>((PPDWORD)p_GameEventManager2, true);
	g_pMaterialSystemHook = std::make_unique<VFTableHook>((PPDWORD)p_MaterialSystem, true);
	g_pDMEHook = std::make_unique<VFTableHook>((PPDWORD)p_ModelRender, true);
	//g_pTraceRayHook = std::make_unique<VFTableHook>((PPDWORD)p_EngineTrace, true);

	o_PaintTraverse = g_pPaintTraverseHook->Hook(41, (PaintTraverse_t)Handlers::PaintTraverse_h);
	o_CreateMove = g_pCreateMoveHook->Hook(24, (CreateMove_t)Handlers::CreateMove_h);
	o_PlaySound = g_pPlaySoundHook->Hook(82, (PlaySound_t)Handlers::PlaySound_h);
	//o_RenderView = g_pRenderViewHook->Hook(6, (RenderView_t)Handlers::RenderView_h);
	o_EndScene = g_pD3DDevHook->Hook(42, (EndScene_t)Handlers::EndScene_h);
	o_Reset = g_pD3DDevHook->Hook(16, (Reset_t)Handlers::Reset_h);
	o_FrameStageNotify = g_pClientHook->Hook(36, (FrameStageNotify_t)Handlers::FrameStageNotify_h);
	o_FireEventClientSide = g_pGameEventManagerHook->Hook(9, (FireEventClientSide_t)Handlers::FireEventClientSide_h);
	o_BeginFrame = g_pMaterialSystemHook->Hook(42, (BeginFrame_t)Handlers::BeginFrame_h);
	//o_DispatchUserMessage = g_pClientHook->Hook(37, (DispatchUserMessage_t)Handlers::DispatchUserMessage_h);
	o_DrawModelExecute = g_pDMEHook->Hook(21, (DrawModelExecute_t)Handlers::DrawModelExecute_h);
	//o_TraceRay = g_pTraceRayHook->Hook(5, (TraceRay_t)Handlers::TraceRay_h);

	// proxies
	applyProxies();

	// events
	roundStartEvent = new RoundStartEvent;
	playerHurtEvent = new PlayerHurtEvent;
	roundStartEvent->registerSelf();
	playerHurtEvent->registerSelf();

	rebGameMovement = new RebuildGameMovement;
	lagComp = new LagCompensation;
	lagComp->initLagRecord();

	/*static bool bInitCvar = false;
	if (!bInitCvar)
	{
		ConVar *cv = p_Console->FindVar("cl_show_usermessage");
		*(int*)(DWORD)&cv->nFlags = 0;

		bInitCvar = true;
	}*/
}

bool applyProxies()
{
	bool success = false;

	success = netMngr->hookProp("CSmokeGrenadeProjectile", "m_bDidSmokeEffect", Proxies::didSmokeEffect, o_didSmokeEffect);
	success = netMngr->hookProp("CBaseViewModel", "m_nSequence", Proxies::viewModelSequence, o_viewModelSequence);
	//success = netMngr->hookProp("CCSPlayer", "m_flLowerBodyYawTarget", Proxies::lowerBodyYawTarget, o_lowerBodyYawTarget);

	return success;
}

void installGladiator()
{
	init();
}

void uninstallGladiator()
{
	SetWindowLongPtr(prozor, GWLP_WNDPROC, (LONG_PTR)oldWindowProc);

	g_pPaintTraverseHook->Unhook(41);
	g_pCreateMoveHook->Unhook(24);
	g_pPlaySoundHook->Unhook(82);
	//g_pRenderViewHook->Unhook(6);
	g_pD3DDevHook->Unhook(42);
	g_pD3DDevHook->Unhook(16);
	g_pClientHook->Unhook(36);
	g_pGameEventManagerHook->Unhook(9);
	g_pMaterialSystemHook->Unhook(42);
	g_pDMEHook->Unhook(21);
	//g_pTraceRayHook->Unhook(5);
}