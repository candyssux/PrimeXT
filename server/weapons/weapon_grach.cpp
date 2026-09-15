/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_grach.h"
#include "weapon_layer.h"
#include "weapons/grach.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_grach, CGRACH);

CGRACH::CGRACH()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CGRACHWeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CGRACH::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(GRACH_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(edict(), "models/w_grach.mdl");
	FallInit();// get ready to fall down.
}

void CGRACH::Precache(void)
{
	PRECACHE_MODEL("models/v_grach.mdl");
	PRECACHE_MODEL("models/w_grach.mdl");
	PRECACHE_MODEL("models/p_grach.mdl");
	PRECACHE_MODEL("models/shell9x19.mdl"); // brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_grach3.wav"); //handgun
}
