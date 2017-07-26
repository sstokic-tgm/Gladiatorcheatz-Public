#pragma once

class ClientEntList
{
public:
	virtual void Function0() = 0;
	virtual void Function1() = 0;
	virtual void Function2() = 0;

	virtual void *GetClientEntity(int iIndex) = 0;
	virtual void *GetClientEntityFromHandle(DWORD hHandle) = 0;
	virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int GetHighestEntityIndex() = 0;
};

extern ClientEntList *p_ClientEntList;