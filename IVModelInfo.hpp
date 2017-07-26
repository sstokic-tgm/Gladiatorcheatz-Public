#pragma once

#include "Vector3.hpp"

#define Assert(_exp) ((void)0)
#define Assert(_exp) ((void)0)

typedef Vector3 QAngle;

struct mstudiobbox_t;
class IMaterial;
struct vcollide_t;
struct model_t;
struct studiohdr_t;
struct virtualmodel_t;
typedef unsigned char byte;
class CPhysCollide;
typedef unsigned short MDLHandle_t;
class CUtlBuffer;
class IClientRenderable;
class Quaternion;
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct mstudiobodyparts_t;
struct mstudiotexture_t;

struct model_t
{
	char name[255];
};

enum Hitboxes
{
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_SPINE,
	HITBOX_LEGS,
	HITBOX_ARMS,
};

enum Bone
{
	BONE_PELVIS = 0,
	LEAN_ROOT,
	CAM_DRIVER,
	BONE_HIP,
	BONE_LOWER_SPINAL_COLUMN,
	BONE_MIDDLE_SPINAL_COLUMN,
	BONE_UPPER_SPINAL_COLUMN,
	BONE_NECK,
	BONE_HEAD,
};

class RadianEuler
{
public:
	inline RadianEuler(void) { }
	inline RadianEuler(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline RadianEuler(Quaternion const &q);
	inline RadianEuler(QAngle const &angles);

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) { x = ix; y = iy; z = iz; }

	QAngle ToQAngle(void) const;
	bool IsValid() const;
	void Invalidate();

	inline float *Base() { return &x; }
	inline const float *Base() const { return &x; }

	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};

class Quaternion
{

public:

	inline Quaternion(void) {}
	inline Quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }
	inline Quaternion(RadianEuler const &angle);

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

	bool IsValid() const;
	void Invalidate();

	bool operator==(const Quaternion &src) const;
	bool operator!=(const Quaternion &src) const;

	float* Base() { return (float*)this; }
	const float* Base() const { return (float*)this; }

	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z, w;
};

struct mstudiobbox_t
{
	int bone;
	int group;
	Vector3 bbmin;
	Vector3 bbmax;
	int hitboxnameindex;
	int pad[3];
	float radius;
	int pad2[4];

	char *getHitboxName()
	{
		if (hitboxnameindex == 0)
			return "";

		return ((char*)this) + hitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int sznameindex;

	inline char *const pszName(void) const
	{
		return ((char*)this) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t *pHitbox(int i) const
	{
		return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i;
	}
};

struct mstudiobone_t
{
	int sznameindex;
	inline char *const pszName(void) const { return ((char *)this) + sznameindex; }
	int parent;
	int bonecontroller[6];

	Vector3 pos;
	Quaternion quat;
	RadianEuler rot;

	Vector3 posscale;
	Vector3 rotscale;

	matrix3x4_t poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex;
	mutable int physicsbone;
	inline void *pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int surfacepropidx;
	inline char *const pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	inline int GetSurfaceProp(void) const { return surfacepropLookup; }

	int contents;
	int surfacepropLookup;
	int unused[7];
};

struct mstudiobonecontroller_t
{
	int bone;
	int type;
	float start;
	float end;
	int rest;
	int inputfield;
	int unused[8];
};

struct studiohdr_t
{
	int id;
	int version;

	long checksum;

	inline const char *pszName(void) const { return name; }
	char name[64];

	int length;

	Vector3 eyeposition;

	Vector3 illumposition;

	Vector3 hull_min;
	Vector3 hull_max;

	Vector3 view_bbmin;
	Vector3 view_bbmax;

	int flags;

	int numbones;
	int boneindex;
	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
	int RemapSeqBone(int iSequence, int iLocalBone) const;
	int RemapAnimBone(int iAnim, int iLocalBone) const;

	int numbonecontrollers;
	int bonecontrollerindex;
	inline mstudiobonecontroller_t *pBonecontroller(int i) const { Assert(i >= 0 && i < numbonecontrollers); return (mstudiobonecontroller_t *)(((byte *)this) + bonecontrollerindex) + i; };

	int numhitboxsets;
	int hitboxsetindex;

	mstudiohitboxset_t *pHitboxSet(int i) const
	{
		Assert(i >= 0 && i < numhitboxsets);
		return (mstudiohitboxset_t *)(((byte *)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t *pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	inline int iHitboxCount(int set) const
	{
		mstudiohitboxset_t const *s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	//private:
	int numlocalanim;
	int localanimindex;
	inline mstudioanimdesc_t *pLocalAnimdesc(int i) const { return NULL; };

	int numlocalseq;
	int localseqindex;
	inline mstudioseqdesc_t *pLocalSeqdesc(int i) const { return NULL; };

	//public:
	bool SequencesAvailable() const
	{
		return true;
	}

	int GetNumSeq() const;
	mstudioanimdesc_t &pAnimdesc(int i) const;
	mstudioseqdesc_t &pSeqdesc(int i) const;
	int iRelativeAnim(int baseseq, int relanim) const;
	int iRelativeSeq(int baseseq, int relseq) const;

	//private:
	mutable int activitylistversion;
	mutable int eventsindexed;
	//public:
	int GetSequenceActivity(int iSequence);
	void SetSequenceActivity(int iSequence, int iActivity);
	int GetActivityListVersion(void);
	void SetActivityListVersion(int version) const;
	int GetEventListVersion(void);
	void SetEventListVersion(int version);

	int numtextures;
	int textureindex;
	inline mstudiotexture_t *pTexture(int i) const { return NULL; };

	int numcdtextures;
	int cdtextureindex;
	inline char *pCdtexture(int i) const { return (((char *)this) + *((int *)(((byte *)this) + cdtextureindex) + i)); };

	int numskinref;
	int numskinfamilies;
	int skinindex;
	inline short *pSkinref(int i) const { return (short *)(((byte *)this) + skinindex) + i; };

	int numbodyparts;
	int bodypartindex;
	inline mstudiobodyparts_t *pBodypart(int i) const { return NULL; };
};

//-----------------------------------------------------------------------------
// Indicates the type of translucency of an unmodulated renderable
//-----------------------------------------------------------------------------
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};

class IVModelInfo
{

public:

	virtual ~IVModelInfo(void) { }
	virtual const model_t *GetModel(int modelindex) const = 0;
	// Returns index of model by name
	virtual int GetModelIndex(const char *name) const = 0;
	// Returns name of model
	virtual const char *GetModelName(const model_t *model) const = 0;
	virtual vcollide_t *GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t *GetVCollide(int modelindex) const = 0;
	virtual void GetModelBounds(const model_t *model, Vector3 &mins, Vector3 &maxs) const = 0;
	virtual	void GetModelRenderBounds(const model_t *model, Vector3 &mins, Vector3 &maxs) const = 0;
	virtual int GetModelFrameCount(const model_t *model) const = 0;
	virtual int GetModelType(const model_t *model) const = 0;
	virtual void *GetModelExtraData(const model_t *model) = 0;
	virtual bool ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool IsTranslucent(model_t const* model) const = 0;
	virtual bool IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void Unused0() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int GetModelMaterialCount(const model_t* model) const = 0;
	virtual void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterials) = 0;
	virtual bool IsModelVertexLit(const model_t *model) const = 0;
	virtual const char *GetModelKeyValueText(const model_t *model) = 0;
	virtual bool GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
	virtual float GetModelRadius(const model_t *model) = 0;

	virtual const studiohdr_t *FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t *FindModel(void *cache) const = 0;
	virtual	virtualmodel_t *GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual byte *GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
	virtual bool HasAnimBlockBeenPreloaded(studiohdr_t const*, int) const = 0;

	// Available on client only!!!
	virtual void GetModelMaterialColorAndLighting(const model_t *model, Vector3 const &origin, Vector3 const &angles, trace_t *pTrace, Vector3 &lighting, Vector3 &matColor) = 0;
	virtual void GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector3 const &origin, Vector3 const &angles, Vector3 *pLightingCenter) = 0;

	virtual int GetModelContents(int modelIndex) const = 0;
	virtual studiohdr_t *GetStudioModel(const model_t *mod) = 0;
	virtual int GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int GetModelSpriteHeight(const model_t *model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char ComputeLevelScreenFade(const Vector3 &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char ComputeViewScreenFade(const Vector3 &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide *GetCollideForVirtualTerrain(int index) = 0;
	virtual bool IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;
	virtual const model_t *FindOrLoadModel(const char *name) const = 0;
	virtual MDLHandle_t GetCacheHandle(const model_t *model) const = 0;
	// Returns planes of non-nodraw brush model surfaces
	virtual int GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector3 *pOrigin) const = 0;
	virtual int GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool UsesStaticLighting(const model_t *model) const = 0;
};

extern IVModelInfo *p_ModelInfo;