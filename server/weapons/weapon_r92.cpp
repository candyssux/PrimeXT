/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_r92.h"
#include "weapon_layer.h"
#include "weapons/r92.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_R92, CR92);

CR92::CR92()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CR92WeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CR92::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(R92_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(ENT(pev), "models/w_r92.mdl");
	FallInit(); // get ready to fall down.
}

void CR92::Precache()
{
	PRECACHE_MODEL("models/v_r92.mdl");
	PRECACHE_MODEL("models/w_r92.mdl");
	PRECACHE_MODEL("models/p_r92.mdl");

	PRECACHE_MODEL("models/ammo_9x18.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("weapons/r92_reload1.wav");
	PRECACHE_SOUND("weapons/r92_cock1.wav");
	PRECACHE_SOUND("weapons/r92_shot1.wav");
	PRECACHE_SOUND("weapons/r92_shot2.wav");

	PRECACHE_MODEL("models/shell.mdl"); // brass shell
}

int CR92::AddToPlayer(CBasePlayer *pPlayer)
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
