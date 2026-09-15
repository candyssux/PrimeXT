/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_aps.h"
#include "weapon_layer.h"
#include "weapons/aps.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_aps, CAPS);

CAPS::CAPS()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CAPSWeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CAPS::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(APS_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(edict(), "models/w_aps.mdl");
	FallInit();// get ready to fall down.
}

void CAPS::Precache(void)
{
	PRECACHE_MODEL("models/v_aps.mdl");
	PRECACHE_MODEL("models/w_aps.mdl");
	PRECACHE_MODEL("models/p_aps.mdl");
	PRECACHE_MODEL("models/shell.mdl"); // brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_aps3.wav"); //handgun
}
