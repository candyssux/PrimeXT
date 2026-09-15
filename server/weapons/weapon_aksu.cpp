/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_aksu.h"
#include "user_messages.h"
#include "weapon_layer.h"
#include "weapons/aksu.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_aksu, CAKSU);

CAKSU::CAKSU()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CAKSUWeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CAKSU::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(AKSU_CLASSNAME));
	Precache();
	SET_MODEL(ENT(pev), "models/w_aksu.mdl");
	FallInit(); // get ready to fall down.
}

void CAKSU::Precache()
{
	PRECACHE_MODEL("models/v_aksu.mdl");
	PRECACHE_MODEL("models/w_aksu.mdl");
	PRECACHE_MODEL("models/p_aksu.mdl");

	PRECACHE_MODEL("models/shell545x39.mdl");// brass shellTE_MODEL

	PRECACHE_MODEL("models/grenade.mdl");	// grenade

	PRECACHE_MODEL("models/ammo_545x39.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");

	PRECACHE_SOUND("weapons/aksu1.wav");// H to the K
	PRECACHE_SOUND("weapons/aksu2.wav");// H to the K
	PRECACHE_SOUND("weapons/aksu3.wav");// H to the K

	PRECACHE_SOUND("weapons/glauncher.wav");
	PRECACHE_SOUND("weapons/glauncher2.wav");

	PRECACHE_SOUND("weapons/357_cock1.wav");
}

int CAKSU::AddToPlayer(CBasePlayer *pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_pWeaponContext->m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}
