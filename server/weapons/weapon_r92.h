/***
*
*	ionization weapons by kpe0
*
****/

#pragma once

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "monsters.h"
#include "player.h"
#include "gamerules.h"
#include "user_messages.h"

class CR92 : public CBasePlayerWeapon
{
	DECLARE_CLASS(CR92, CBasePlayerWeapon);

public:
	CR92();

	void Spawn();
	void Precache();
	int AddToPlayer(CBasePlayer *pPlayer);
};
