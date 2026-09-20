/***
*
*	weapons by kpe0
*
****/

#pragma once
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

class C9X18Ammo : public CBasePlayerAmmo
{
	DECLARE_CLASS(C9X18Ammo, CBasePlayerAmmo);

	void Spawn();
	void Precache();
	BOOL AddAmmo(CBaseEntity *pOther);
};
