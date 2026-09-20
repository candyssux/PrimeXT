#pragma once

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "player.h"
#include "items.h"
#include "gamerules.h"
#include "user_messages.h"

class CHealthKitBig : public CItem
{
	DECLARE_CLASS(CHealthKitBig, CItem);

public:
	void Spawn(void);
	void Precache(void);
	BOOL MyTouch(CBasePlayer *pPlayer);
};