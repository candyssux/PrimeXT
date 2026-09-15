/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_OC33.h"
#include "weapon_layer.h"
#include "weapons/OC33.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_OC33, COC33);

COC33::COC33()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<COC33WeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void COC33::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(OC33_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(edict(), "models/w_oc33.mdl");
	FallInit();// get ready to fall down.
}

void COC33::Precache(void)
{
	PRECACHE_MODEL("models/v_oc33.mdl");
	PRECACHE_MODEL("models/w_oc33.mdl");
	PRECACHE_MODEL("models/p_oc33.mdl");
	PRECACHE_MODEL("models/shell.mdl"); // brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_oc333.wav"); //handgun
}
