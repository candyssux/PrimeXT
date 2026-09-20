#include "item_hkit_big.h"

LINK_ENTITY_TO_CLASS(item_hkit_big, CHealthKitBig);

void CHealthKitBig::Spawn(void)
{
	Precache();
	SET_MODEL(ENT(pev), "models/w_medkit_big.mdl");

	CItem::Spawn();
}

void CHealthKitBig::Precache(void)
{
	PRECACHE_MODEL("models/w_medkit_big.mdl");
	PRECACHE_SOUND("items/smallmedkit1.wav");
}

BOOL CHealthKitBig::MyTouch(CBasePlayer *pPlayer)
{
	if (pPlayer->pev->deadflag != DEAD_NO)
	{
		return FALSE;
	}

	if (pPlayer->TakeHealth(40, DMG_GENERIC))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev);
		WRITE_STRING(STRING(pev->classname));
		MESSAGE_END();

		EMIT_SOUND(
			ENT(pPlayer->pev),
			CHAN_ITEM,
			"items/smallmedkit1.wav",
			1,
			ATTN_NORM
		);

		if (g_pGameRules->ItemShouldRespawn(this))
		{
			Respawn();
		}
		else
		{
			UTIL_Remove(this);
		}

		return TRUE;
	}

	return FALSE;
}