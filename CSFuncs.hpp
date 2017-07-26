#pragma once

#include "harCs.hpp"

#define M_PI 3.14159265358979323846f
#define M_RADPI 57.295779513082f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))

class Entity;
class CntDwnTimer;
class CSFuncs;

/* CntDwnTimer */
class CntDwnTimer
{
public:

	CntDwnTimer();

	void reset();
	void start(float dur);
	bool isElapsed();
	float getRemainingTime() const;
	float getPercentge() const;
	float getTimeLength() const;

private:

	float dur;
	float timestmp;
	virtual float now() const;
	float timeLength;
};

class CSFuncs
{

public:

	HMODULE		hClientDll, hEngineDll;
	DWORD_PTR	clientDllSize, engineDllSize;
	MODULEINFO	modClientDll, modEngineDll;
	DWORD		dClientDll, dEngineDll, dClientDllSize, dEngineDllSize;

	void init();
	CSFuncs() {};
	~CSFuncs();

	DWORD getClientMode();
	DWORD getIsRdy();
	DWORD getD3D();
	DWORD getEntityList();
	DWORD getLocalPlayer();
	DWORD getEngine();
	DWORD getRenderView();
	int getLocPlyrConnect(DWORD base);
	DWORD getEntityById(DWORD id);
	DWORD getGlowObjects();
	DWORD getGlowObjectCount();
	DWORD getInCross();
	DWORD getGameRes();

	char *getWeapName(int idx);
	bool isWeapPistol(int idx);
	bool isWeapSniper(int idx);
	bool isWeapNonAim(int idx);

	void randomSeed(int iSeed);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);

	void normaleAechse(Vector3 &vektRein);
	float distance(Vector3 loc, Vector3 rem);
	Vector3 racunajAechse(Vector3 src, Vector3 dst);
	void aechseVektor(Vector3 aechse, float *forward, float *right, float *up);
	void umiri(Vector3 src, Vector3 &dst, float factor);
	void praviVektor(Vector3 src, Vector3 &dst);
	float getFoV(const Vector3 &viewAechse, const Vector3 &viewUgao);
	void vekAechse(Vector3 &forward, Vector3 &aechse);
	void vekAechse2(Vector3 &forward, Vector3 &aechse);
	void vekAechse(Vector3 &forward, Vector3 &up, Vector3 &aechse);
	bool isVis(Vector3 loc, Vector3 rem, Entity *base);

	DWORD findPattern(char *pattern, char *mask, DWORD start, DWORD end, DWORD offset);

private:

	bool findOffset(DWORD moduleBase, DWORD moduleSize, char *pattern, char *mask, DWORD addressOffset, char *offsetName, DWORD *offsetAddress, bool subtractBase = false);
	void getOffsets(DWORD clientDll, DWORD clientDllSize, DWORD engineDll, DWORD engineDllSize);
	bool findLocalPlayer(DWORD clientDll, DWORD clientDllSize);
	bool findEntityList(DWORD clientDll, DWORD clientDllSize);
};

extern CSFuncs *csFuncs;
extern CntDwnTimer coravTimer;