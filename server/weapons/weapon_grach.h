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

class CGRACH : public CBasePlayerWeapon
{
	DECLARE_CLASS(CGRACH, CBasePlayerWeapon);

public:
	CGRACH();

	void Spawn(void);
	void Precache(void);
};
