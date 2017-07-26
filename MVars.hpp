#pragma once

#include <string>
#include <unordered_map>

extern const char *opt_Key[];
extern const char *opt_Key2[];
extern const char *opt_EspType[];
extern const char *opt_AimSpot[];
extern const char *opt_AAxType[];
extern const char *opt_AAyType[];
extern const char *opt_TargetType[];
extern int realAimSpot[];
extern int realHitboxSpot[];
extern int realFakeLagChoke[];
extern int realFakeLagSend[];

struct WeaponSettings;

class MVars
{
public:

	int open = 1;

	int umiriFactor = 1;
	int maxFov = 1;
	float hvhFov = 1.f;

	int kAim = 0;
	int kAim2 = 0;
	int preAim = 1;
	int afterShots = 3;
	int afterAim = 1;
	int hitAim = 0;
	bool bEnableCekaj = false;
	int cekajTasta = false;
	int bEnableTargetType = 0;
	bool bEnableGlow = false;
	bool bEnableDlight = false;
	bool bEnableTrig = false;
	bool bEnableRK = false;
	bool bEnableSkelet = false;
	bool bEnableRage = false;
	int bEnableEspBoxType = 0;
	bool bEnableEspName = false;
	bool bEnableEspMMinfo = false;
	bool bEnableEspWeapon = false;
	bool bEnableAA = false;
	bool bEnableNoFlash = false;
	bool bEnableNoSmoke = false;
	bool bEnableEspBomba = false;
	bool bEnableTrigWithKey = false;
	bool bEnableDrawAimSpot = false;
	bool bEnableEspIgnTeam = false;
	bool bEnableSnipCrosshair = false;
	bool bEnableDrawSkelet = false;
	bool bEnableSnapLine = false;
	bool bEnableNadeTracer = false;
	bool bEnableBhop = false;
	bool bEnableAS = false;
	bool bEnableRankRev = false;
	int bEnableTypeX = 0;
	int bEnableTypeY = 0;
	int bFakeTypeY = 0;
	bool bEnableFakeLag = false;
	float wallbangDmg = 0.f;
	float hitsansaAmnt = 0.f;
	bool bWallbangBones[6] = { true, false, false, false, false, false };
	bool bHitscanHitbox = false;
	bool bHitscan = false;
	bool bEnableCucati = false;
	bool bEnableStati = false;
	bool bEnableAutoPucanje = false;
	bool bEnableAutoZum = false;
	bool bEnableSpam = false;
	int fakeLagChoke = 1;
	int fakeLagSend = 1;
	bool bEnableResolve = false;
	bool bEnableSkinChange = false;
	int resTicks = 1;
	int resModulo = 1;
	bool bEnableSpec = false;
	bool bSilent = false;
	bool bEnableWireframe = false;
	int historyIdx = 0;
	bool bEnableLagComp = false;
	bool bNoVisRec = false;
	bool bEnableLagCompSkelet = false;
	bool bEnableHitMarkers = false;
	bool bEnableChatBot = false;
	bool bLbyBreaker = false;

	std::unordered_map<int, WeaponSettings> weapons;
};

struct WeaponSettings
{
	// legit
	bool bEnableSkelet, bEnableRK, bEnableTrig, bEnableTrigWithKey;
	int kAim, kAim2, preAim, afterShots, afterAim, maxFov, umiriFactor;

	// rage
	bool bEnableRage, bEnableCekaj, bSilent, bEnableAutoPucanje, bEnableAutoZum, bEnableCucati, bEnableStati, bHitscanHitbox, bHitscan;
	bool bWallbangBones[6];
	int cekajTasta, bEnableTargetType, hitAim;
	float hvhFov, wallbangDmg, hitsansaAmnt;

	WeaponSettings(
		// legit
		bool bEnableSkelet, bool bEnableRK, bool bEnableTrig, bool bEnableTrigWithKey, int kAim, int kAim2, int preAim, int afterShots, int afterAim, int maxFov,
		int umiriFactor,
		// rage
		bool bEnableRage, bool bEnableCekaj, bool bSilent, bool bEnableAutoPucanje, bool bEnableAutoZum, bool bEnableCucati, bool bEnableStati, bool bHitscanHitbox,
		bool bHitscan, int cekajTasta, int bEnableTargetType, int hitAim, float hvhFov, float wallbangDmg, float hitsansaAmnt, bool bWallbangBones[6] = nullptr
		)
	{
		// legit
		this->bEnableSkelet = bEnableSkelet;
		this->bEnableRK = bEnableRK;
		this->bEnableTrig = bEnableTrig;
		this->bEnableTrigWithKey = bEnableTrigWithKey;
		this->kAim = kAim;
		this->kAim2 = kAim2;
		this->preAim = preAim;
		this->afterShots = afterShots;
		this->afterAim = afterAim;
		this->maxFov = maxFov;
		this->umiriFactor = umiriFactor;
		// rage
		this->bEnableRage = bEnableRage;
		this->bEnableCekaj = bEnableCekaj;
		this->bSilent = bSilent;
		this->bEnableAutoPucanje = bEnableAutoPucanje;
		this->bEnableAutoZum = bEnableAutoZum;
		this->bEnableCucati = bEnableCucati;
		this->bEnableStati = bEnableStati;
		this->bHitscanHitbox = bHitscanHitbox;
		this->bHitscan = bHitscan;
		this->cekajTasta = cekajTasta;
		this->bEnableTargetType = bEnableTargetType;
		this->hitAim = hitAim;
		this->hvhFov = hvhFov;
		this->wallbangDmg = wallbangDmg;
		this->hitsansaAmnt = hitsansaAmnt;

		for (int i = 0; i <= 5; i++)
			this->bWallbangBones[i] = bWallbangBones != nullptr ? bWallbangBones[i] : false;
	}

	WeaponSettings() {};

	bool operator==(const WeaponSettings &other) const
	{
		for (int i = 0; i <= 5; i++)
			if (this->bWallbangBones[i] != other.bWallbangBones[i])
				return false;

		return 
			this->bEnableSkelet == other.bEnableSkelet &&
			this->bEnableRK == other.bEnableRK &&
			this->bEnableTrig == other.bEnableTrig &&
			this->bEnableTrigWithKey == other.bEnableTrigWithKey &&
			this->kAim == other.kAim &&
			this->kAim2 == other.kAim2 &&
			this->preAim == other.preAim &&
			this->afterShots == other.afterShots &&
			this->afterAim == other.afterAim &&
			this->maxFov == other.maxFov &&
			this->umiriFactor == other.umiriFactor &&
		
			this->bEnableRage == other.bEnableRage &&
			this->bEnableCekaj == other.bEnableCekaj &&
			this->bSilent == other.bSilent &&
			this->bEnableAutoPucanje == other.bEnableAutoPucanje &&
			this->bEnableAutoZum == other.bEnableAutoZum &&
			this->bEnableCucati == other.bEnableCucati &&
			this->bEnableStati == other.bEnableStati &&
			this->bHitscanHitbox == other.bHitscanHitbox &&
			this->bHitscan == other.bHitscan &&
			this->cekajTasta == other.cekajTasta &&
			this->bEnableTargetType == other.bEnableTargetType &&
			this->hitAim == other.hitAim &&
			this->hvhFov == other.hvhFov &&
			this->wallbangDmg == other.wallbangDmg &&
			this->hitsansaAmnt == other.hitsansaAmnt;
	}
};

extern MVars mVars;