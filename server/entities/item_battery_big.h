#pragma once

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"
#include "skill.h"
#include "items.h"
#include "user_messages.h"

class CItemBatteryBig : public CItem
{
	void Spawn(void);
	void Precache(void);
	BOOL MyTouch(CBasePlayer *pPlayer);
};