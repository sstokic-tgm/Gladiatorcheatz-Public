#pragma once

#include "CRC.hpp"
#include "Vector3.hpp"

#define IN_ATTACK (1 << 0)
#define IN_JUMP (1 << 1)
#define IN_DUCK (1 << 2)
#define IN_FORWARD (1 << 3)
#define IN_BACK (1 << 4)
#define IN_USE (1 << 5)
#define IN_CANCEL (1 << 6)
#define IN_LEFT (1 << 7)
#define IN_RIGHT (1 << 8)
#define IN_MOVELEFT (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2 (1 << 11)
#define IN_RUN (1 << 12)
#define IN_RELOAD (1 << 13)
#define IN_ALT1 (1 << 14)
#define IN_ALT2 (1 << 15)
#define IN_SCORE (1 << 16)
#define IN_SPEED (1 << 17)
#define IN_WALK (1 << 18)
#define IN_ZOOM (1 << 19)
#define IN_WEAPON1 (1 << 20)
#define IN_WEAPON2 (1 << 21)
#define IN_BULLRUSH (1 << 22)
#define IN_GRENADE1 (1 << 23)
#define IN_GRENADE2 (1 << 24)
#define IN_LOOKSPIN (1 << 25)

#define FL_ONGROUND (1 << 0)
#define FL_DUCKING (1 << 1)
#define FL_WATERJUMP (1 << 2)
#define FL_ONTRAIN (1 << 3)
#define FL_INRAIN (1 << 4)
#define FL_FROZEN (1 << 5)
#define FL_ATCONTROLS (1 << 6)
#define FL_CLIENT (1 << 7)
#define FL_FAKECLIENT (1 << 8)

class UserCMD
{
public:

	virtual ~UserCMD() {};

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;
		CRC32_Init(&crc);

		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

		CRC32_Final(&crc);
		return crc;
	}

	int command_number;
	int tick_count;
	Vector3 viewangles;
	Vector3 aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	char pad_0x4C[0x18];

};

class VerifiedUserCMD
{
public:

	UserCMD  m_cmd;
	CRC32_t   m_crc;
};

class Input
{
public:

	void *pvftable;
	bool m_fTrackIRAvailable;
	bool m_fMouseInitialized;
	bool m_fMouseActive;
	bool m_fJoystickAdvancedInit;
	char pad_0x08[0x2C];
	void *m_pKeys;
	char pad_0x38[0x64];
	bool m_fCameraInterceptingMouse;
	bool m_fCameraInThirdPerson;
	bool m_fCameraMovingWithMouse;
	Vector3 m_vecCameraOffset;
	bool m_fCameraDistanceMove;
	int m_nCameraOldX;
	int m_nCameraOldY;
	int m_nCameraX;
	int m_nCameraY;
	bool m_CameraIsOrthographic;
	Vector3 m_angPreviousViewAngles;
	Vector3 m_angPreviousViewAnglesTilt;
	float m_flLastForwardMove;
	int m_nClearInputState;
	char pad_0xE4[0x8];
	UserCMD *m_pCommands;
	VerifiedUserCMD *m_pVerifiedCommands;
};

extern Input *p_Input;