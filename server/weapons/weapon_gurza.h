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

class CGURZA : public CBasePlayerWeapon
{
	DECLARE_CLASS(CGURZA, CBasePlayerWeapon);

public:
	CGURZA();

	void Spawn(void);
	void Precache(void);
};
