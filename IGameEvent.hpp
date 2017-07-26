#pragma once

class GameEventListener2;
class GameEventManager2;
class GameEvent;
class bf_read;
class bf_write;

#define EVENT_DEBUG_ID_INIT 0x2A

class GameEvent
{
public:

	virtual ~GameEvent() {};
	virtual const char *getName() const = 0;
	virtual bool  isReliable() const = 0;
	virtual bool  isLocal() const = 0;
	virtual bool  isEmpty(const char *keyName = NULL) = 0;
	virtual bool  getBool(const char *key = NULL, bool def = false) = 0;
	virtual int   getInt(const char *key = NULL, int def = 0) = 0;
	virtual unsigned long long getUInt64(char const *key = NULL, unsigned long long def = 0) = 0;
	virtual float getFloat(const char *key = NULL, float def = 0.0f) = 0;
	virtual const char *getString(const char *key = NULL, const char *def = "") = 0;
	virtual const wchar_t *getWString(char const *key = NULL, const wchar_t *def = L"") = 0;
	virtual void unk0() = 0;
	virtual void setBool(const char *key, bool val) = 0;
	virtual void setInt(const char *key, int val) = 0;
	virtual void setUInt64(const char *key, unsigned long long val) = 0;
	virtual void setFloat(const char *key, float val) = 0;
	virtual void setString(const char *key, const char *val) = 0;
	virtual void setWString(const char *key, const wchar_t *val) = 0;
};

class GameEventListener2
{
public:

	virtual	~GameEventListener2(void) {};

	virtual void FireGameEvent(GameEvent *gEvent) = 0;
	virtual int  GetEventDebugID(void) = 0;
};

class GameEventManager2
{
public:

	virtual	~GameEventManager2(void) {};

	virtual int LoadEventsFromFile(const char *filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(GameEventListener2 *listener, const char *name, bool bServerSide) = 0;
	virtual bool FindListener(GameEventListener2 *listener, const char *name) = 0;
	virtual void RemoveListener(GameEventListener2 *listener) = 0;
	virtual GameEvent *CreateGameEvent(const char *name, bool bForce = false) = 0;
	virtual bool FireEvent(GameEvent *event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(GameEvent *gEvent) = 0;
	virtual GameEvent *DuplicateEvent(GameEvent *gEvent) = 0;
	virtual void FreeEvent(GameEvent *gEvent) = 0;
	virtual bool SerializeEvent(GameEvent *gEvent, bf_write *buf) = 0;
	virtual GameEvent *UnserializeEvent(bf_read *buf) = 0;
};

extern GameEventManager2 *p_GameEventManager2;