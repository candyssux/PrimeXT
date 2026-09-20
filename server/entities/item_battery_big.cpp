#include "item_battery_big.h"

LINK_ENTITY_TO_CLASS(item_battery_big, CItemBatteryBig);

void CItemBatteryBig::Spawn(void)
{
	Precache();

	if (pev->model)
		SET_MODEL(ENT(pev), STRING(pev->model));
	else
		SET_MODEL(ENT(pev), "models/w_battery_big.mdl");

	CItem::Spawn();
}

void CItemBatteryBig::Precache(void)
{
	if (pev->model)
		PRECACHE_MODEL((char*)STRING(pev->model));
	else
		PRECACHE_MODEL("models/w_battery_big.mdl");

	if (pev->noise)
		PRECACHE_SOUND((char*)STRING(pev->noise));
	else
		PRECACHE_SOUND("items/gunpickup2.wav");
}

BOOL CItemBatteryBig::MyTouch(CBasePlayer *pPlayer)
{
	if (pPlayer->pev->deadflag != DEAD_NO)
	{
		return FALSE;
	}

	if ((pPlayer->pev->armorvalue < MAX_NORMAL_BATTERY) && pPlayer->HasWeapon(WEAPON_SUIT))
	{
		int pct;
		char szcharge[64];

		pPlayer->pev->armorvalue += 40;
		pPlayer->pev->armorvalue = Q_min(pPlayer->pev->armorvalue, MAX_NORMAL_BATTERY);

		if (pev->noise)
			EMIT_SOUND(pPlayer->edict(), CHAN_ITEM, STRING(pev->noise), 1, ATTN_NORM);
		else
			EMIT_SOUND(pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM);

		MESSAGE_BEGIN(MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev);
		WRITE_STRING(STRING(pev->classname));
		MESSAGE_END();

		pct = (int)((float)(pPlayer->pev->armorvalue * 100.0) * (1.0 / MAX_NORMAL_BATTERY) + 0.5);
		pct = (pct / 5);

		if (pct > 0)
			pct--;

		sprintf(szcharge, "!HEV_%1dP", pct);

		pPlayer->SetSuitUpdate(szcharge, FALSE, SUIT_NEXT_IN_30SEC);

		return TRUE;
	}

	return FALSE;
}