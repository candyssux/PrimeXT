/***
*
*	weapons by kpe0
*
****/

#include "ammo_9x18.h"

LINK_ENTITY_TO_CLASS(ammo_9X18, C9X18Ammo);

void C9X18Ammo::Spawn()
{
	Precache();
	SET_MODEL(ENT(pev), "models/w_ammo_9x18.mdl");
	CBasePlayerAmmo::Spawn();
}

void C9X18Ammo::Precache()
{
	PRECACHE_MODEL("models/w_ammo_9x18.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");
}

BOOL C9X18Ammo::AddAmmo(CBaseEntity *pOther)
{
	if (pOther->GiveAmmo(16, "9X18", _9X18_MAX_CARRY) != -1)
	{
		EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		return TRUE;
	}
	return FALSE;
}
