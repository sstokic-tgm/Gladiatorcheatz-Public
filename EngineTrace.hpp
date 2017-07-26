#pragma once

#include <d3dx9math.h>
#include "VT.hpp"

// Hitgroups
#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7

#define DISPSURF_FLAG_SURFACE (1<<0)
#define DISPSURF_FLAG_WALKABLE (1<<1)
#define DISPSURF_FLAG_BUILDABLE (1<<2)
#define DISPSURF_FLAG_SURFPROP1 (1<<3)
#define DISPSURF_FLAG_SURFPROP2 (1<<4)

#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

#define	MASK_SHOT	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	SURF_NODECALS	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID	(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

class GTrace;
typedef GTrace trace_t;

struct surfacephysicsparams_t
{
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
};

struct surfaceaudioparams_t
{
	float reflectivity;
	float hardnessFactor;
	float roughnessFactor;
	float roughThreshold;
	float hardThreshold;
	float hardVelocityThreshold;
};

struct surfacesoundnames_t
{
	unsigned short walkStepLeft;
	unsigned short walkStepRight;
	unsigned short runStepLeft;
	unsigned short runStepRight;
	unsigned short impactSoft;
	unsigned short impactHard;
	unsigned short scrapeSmooth;
	unsigned short scrapeRough;
	unsigned short bulletImpact;
	unsigned short rolling;
	unsigned short reakSound;
	unsigned short strainSound;
};

struct surfacesoundhandles_t
{
	short walkStepLeft;
	short walkStepRight;
	short runStepLeft;
	short runStepRight;
	short impactSoft;
	short impactHard;
	short scrapeSmooth;
	short scrapeRough;
	short bulletImpact;
	short rolling;
	short breakSound;
	short strainSound;
};

struct surfacegameprops_t
{
public:
	float maxSpeedFactor;
	float jumpFactor;
	char pad00[0x4];
	float flPenetrationModifier;
	float flDamageModifier;
	unsigned short material;
	char pad01[0x3];
};

struct surfacedata_t
{
	surfacephysicsparams_t physics;
	surfaceaudioparams_t audio;
	surfacesoundnames_t sounds;
	surfacegameprops_t game;
	surfacesoundhandles_t soundhandles;
};

enum class TraceType_t {

	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct cplane_t
{
	Vector3 normal;
	float dist;
	BYTE type;
	BYTE signbits;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class BTrace
{
public:

	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	Vector3 startpos;
	Vector3 endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;

	BTrace() {}
};

class GTrace : public BTrace
{
public:

	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IV() const;

public:

	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	void *m_pEntityHit;
	int hitbox;

	GTrace() {}

private:

	GTrace(const GTrace &vOther);

	/*GTrace(const GTrace &other) : fractionleftsolid(other.fractionleftsolid), surface(other.surface), hitgroup(other.hitgroup), physicsbone(other.physicsbone), worldSurfaceIndex(other.worldSurfaceIndex), m_pEntityHit(other.m_pEntityHit), hitbox(other.hitbox)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
	}

	GTrace &GTrace::operator=(const GTrace &other)
	{
		startpos = other.startpos;
		endpos = other.endpos;
		plane = other.plane;
		fraction = other.fraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allsolid = other.allsolid;
		startsolid = other.startsolid;
		fractionleftsolid = other.fractionleftsolid;
		surface = other.surface;
		hitgroup = other.hitgroup;
		physicsbone = other.physicsbone;
		worldSurfaceIndex = other.worldSurfaceIndex;
		m_pEntityHit = other.m_pEntityHit;
		hitbox = other.hitbox;
		return *this;
	}*/
};

/*inline bool GTrace::DH() const {

	return fraction < 1 || allsolid || startsolid;
}*/

inline bool GTrace::IV() const {

	return fraction > 0.97f;
}

struct Ray_t
{
	Vector3Aligned  m_Start;
	Vector3Aligned  m_Delta;
	Vector3Aligned  m_StartOffset;
	Vector3Aligned  m_Extents;

	const   matrix3x4_t *m_pWorldAxisTransform;

	bool    m_IsRay;
	bool    m_IsSwept;

	Ray_t() : m_pWorldAxisTransform(NULL) { }

	void Init(Vector3 &vecStart, Vector3 &vecEnd)
	{
		m_Delta = Vector3Aligned(vecEnd - vecStart);

		m_IsSwept = (m_Delta.lengthSqr() != 0);

		m_Extents.clearVekt();

		m_pWorldAxisTransform = NULL;

		m_IsRay = true;

		m_StartOffset.clearVekt();

		m_Start = vecStart;
	}

	void Init(Vector3 const &start, Vector3 const &end, Vector3 const &mins, Vector3 const &maxs)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.lengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.lengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}
};

class TraceFilter
{
public:

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t	GetTraceType()
	{
		return TraceType_t::TRACE_EVERYTHING;
	}
	void *pSkip;
};

class TraceFilterSkipTwoEntities : public TraceFilter
{
public:

	TraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
	{
		pSkip1 = pPassEnt1;
		pSkip2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask)
	{
		return  !(pEntityHandle == pSkip1 || pEntityHandle == pSkip2);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}

	void *pSkip1;
	void *pSkip2;
};

class TraceFilterAutowall : public TraceFilter
{
public:

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask)
	{
		return  pEntityHandle != pSkip1 && pEntityHandle != pSkip2;
	}

	void *pSkip1;
	void *pSkip2;
};

class TraceFilterSkipCurrentEntity : public TraceFilter
{
public:

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}

	void *pSkip;
};

class ITraceListData;
class ICollideable;
class CPhysCollide;
class IHandleEntity;
class IEntityEnumerator;
struct virtualmeshlist_t;
struct BrushSideInfo_t;

enum DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

template <class T, class I = int>
class CUtlMemory
{
public:

	T& operator[](I i) {
		return m_pMemory[i];
	}

protected:

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

template <class T, class A = CUtlMemory<T>>
class CUtlVector
{
	typedef A CAllocator;

public:

	T& operator[](int i) {
		return m_Memory[i];
	}

	int Count() const {
		return m_Size;
	}

protected:

	CAllocator m_Memory;
	int m_Size;
	T* m_pElements;
};

class EngineTrace
{
public:

	/*void TraceRay(const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, GTrace *pTrace)
	{
		typedef void(__thiscall* o_TraceRay)(void*, const Ray_t&, unsigned int, TraceFilter *, GTrace*);
		VT::vfunc<o_TraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}*/

	virtual int GetPointContents(const Vector3 &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = NULL) = 0;
	virtual int GetPointContents_WorldOnly(const Vector3 &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int GetPointContents_Collideable(ICollideable *pCollide, const Vector3 &vecAbsPosition) = 0;
	virtual void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace) = 0;
	virtual void ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, trace_t *pTrace) = 0;
	virtual void TraceRay(const Ray_t &ray, unsigned int fMask, TraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void SetupLeafAndEntityListRay(const Ray_t &ray, ITraceListData *pTraceData) = 0;
	virtual void SetupLeafAndEntityListBox(const Vector3 &vecBoxMin, const Vector3 &vecBoxMax, ITraceListData *pTraceData) = 0;
	virtual void TraceRayAgainstLeafAndEntityList(const Ray_t &ray, ITraceListData *pTraceData, unsigned int fMask, TraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void SweepCollideable(ICollideable *pCollide, const Vector3 &vecAbsStart, const Vector3 &vecAbsEnd, const Vector3 &vecAngles, unsigned int fMask, TraceFilter *pTraceFilter, trace_t *pTrace) = 0;
	virtual void EnumerateEntities(const Ray_t &ray, bool triggers, IEntityEnumerator *pEnumerator) = 0;
	virtual void EnumerateEntities(const Vector3 &vecAbsMins, const Vector3 &vecAbsMaxs, IEntityEnumerator *pEnumerator) = 0;
	virtual ICollideable *GetCollideable(IHandleEntity *pEntity) = 0;
	virtual int GetStatByIndex(int index, bool bClear) = 0;
	virtual void GetBrushesInAABB(const Vector3 &vMins, const Vector3 &vMaxs, CUtlVector<int> *pOutput, int iContentsMask = 0xFFFFFFFF) = 0;
	virtual CPhysCollide* GetCollidableFromDisplacementsInAABB(const Vector3& vMins, const Vector3& vMaxs) = 0;
	virtual int GetNumDisplacements() = 0;
	virtual void GetDisplacementMesh(int nIndex, virtualmeshlist_t *pMeshTriList) = 0;
	virtual bool GetBrushInfo(int iBrush, CUtlVector<BrushSideInfo_t> *pBrushSideInfoOut, int *pContentsOut) = 0;
	virtual bool PointOutsideWorld(const Vector3 &ptTest) = 0;
	virtual int GetLeafContainingPoint(const Vector3 &ptTest) = 0;
	virtual ITraceListData *AllocTraceListData() = 0;
	virtual void FreeTraceListData(ITraceListData *) = 0;
	virtual int GetSetDebugTraceCounter(int value, DebugTraceCounterBehavior_t behavior) = 0;
};

extern EngineTrace *p_EngineTrace;