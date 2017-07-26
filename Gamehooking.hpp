#pragma once

#include "harCs.hpp"
#include "VMTManager.hpp"
#include "IfaceMngr.hpp"

extern std::unique_ptr<VFTableHook> g_pPaintTraverseHook;
extern std::unique_ptr<VFTableHook> g_pCreateMoveHook;
extern std::unique_ptr<VFTableHook> g_pPlaySoundHook;
extern std::unique_ptr<VFTableHook> g_pRenderViewHook;
extern std::unique_ptr<VFTableHook> g_pD3DDevHook;
extern std::unique_ptr<VFTableHook> g_pClientHook;
extern std::unique_ptr<VFTableHook> g_pGameEventManagerHook;
extern std::unique_ptr<VFTableHook> g_pMaterialSystemHook;
extern std::unique_ptr<VFTableHook> g_pDMEHook;
extern std::unique_ptr<VFTableHook> g_pTraceRayHook;

extern HWND prozor;
extern WNDPROC oldWindowProc;
extern bool d3dinit;

extern bool pressedKey[256];

/* PaintTraverse defs */
typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
extern PaintTraverse_t o_PaintTraverse;

/* CreateMove defs */
typedef bool(__thiscall *CreateMove_t)(ClientMode*, float, UserCMD*);
extern CreateMove_t o_CreateMove;

/* PlaySound defs */
typedef void(__thiscall *PlaySound_t)(Surface*, const char*);
extern PlaySound_t o_PlaySound;

/* RenderView defs */
typedef void(__thiscall *RenderView_t)(void*, CViewSetup&, CViewSetup&, int, int);
extern RenderView_t o_RenderView;

/* EndScene defs */
typedef long(__stdcall *EndScene_t)(IDirect3DDevice9*);
extern EndScene_t o_EndScene;

/* Reset defs */
typedef long(__stdcall *Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
extern Reset_t o_Reset;

/* FrameStageNotify defs */
typedef void(__stdcall *FrameStageNotify_t)(ClientFrameStage_en);
extern FrameStageNotify_t o_FrameStageNotify;

/* FireEventClientSide defs */
typedef bool(__thiscall *FireEventClientSide_t)(void*, GameEvent*);
extern FireEventClientSide_t o_FireEventClientSide;

/* BeginFrame defs */
typedef void(__thiscall *BeginFrame_t)(void*, float);
extern BeginFrame_t o_BeginFrame;

/* DispatchUserMessage defs */
typedef bool(__thiscall *DispatchUserMessage_t)(void*, int, unsigned int, unsigned int, const void*);
extern DispatchUserMessage_t o_DispatchUserMessage;

/* DrawModelExecute defs */
typedef void(__thiscall *DrawModelExecute_t)(void*, void*, const DrawModelState_t &, const ModelRenderInfo_t &, matrix3x4_t*);
extern DrawModelExecute_t o_DrawModelExecute;

/* TraceRay defs */
typedef void(__thiscall *TraceRay_t)(void*, const Ray_t &, unsigned int, TraceFilter *, trace_t *);
extern TraceRay_t o_TraceRay;

namespace Handlers
{
	void __fastcall PaintTraverse_h(void *thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	bool __stdcall CreateMove_h(float smt, UserCMD *userCMD);
	void __stdcall PlaySound_h(const char *folderIme);
	void __fastcall RenderView_h(void *thisptr, void*, CViewSetup &vSetup, CViewSetup &hvSetup, int nClearFlags, int whatToDraw);
	HRESULT __stdcall EndScene_h(IDirect3DDevice9 *pDevice);
	HRESULT __stdcall Reset_h(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
	LRESULT __stdcall WndProc_h(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void __stdcall FrameStageNotify_h(ClientFrameStage_en stage);
	bool __fastcall FireEventClientSide_h(void *thisptr, void*, GameEvent *gEvent);
	void __fastcall BeginFrame_h(void *thisptr, void*, float ft);
	bool __fastcall DispatchUserMessage_h(void *thisptr, void*, unsigned int msg_type, unsigned int unk1, unsigned int nBytes, const void *msg_data);
	void __fastcall DrawModelExecute_h(void *thisptr, void*, void *ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	void __fastcall TraceRay_h(void *thisptr, void*, const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, trace_t *pTrace);
}

namespace Proxies
{
	void didSmokeEffect(const PrimitiProxyData *pData, void *pStruct, void *pOut);
	void viewModelSequence(const PrimitiProxyData *pData, void *pStruct, void *pOut);
	void lowerBodyYawTarget(const PrimitiProxyData *pData, void *pStruct, void *pOut);
}

extern PrimitiVarProxyFn o_didSmokeEffect;
extern PrimitiVarProxyFn o_viewModelSequence;
extern PrimitiVarProxyFn o_lowerBodyYawTarget;

bool applyProxies();

using IsRdyFn = void(__cdecl*)();
extern IsRdyFn IsRdy;

using RevealRanksFn = bool(__cdecl*)(float*);
extern RevealRanksFn RevealRanks;

using LineThroughSmokeFn = bool(__cdecl*)(Vector3, Vector3, int16_t);
extern LineThroughSmokeFn LineThroughSmoke;