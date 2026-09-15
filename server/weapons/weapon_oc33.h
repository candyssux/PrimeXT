/***
*
*	ionization weapons by kpe0
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

class COC33 : public CBasePlayerWeapon
{
	DECLARE_CLASS(COC33, CBasePlayerWeapon);

public:
	COC33();

	void Spawn(void);
	void Precache(void);
};
