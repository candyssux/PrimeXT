/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_gurza.h"
#include "weapon_layer.h"
#include "weapons/gurza.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_gurza, CGURZA);

CGURZA::CGURZA()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CGURZAWeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CGURZA::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(GURZA_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(edict(), "models/w_gurza.mdl");
	FallInit();// get ready to fall down.
}

void CGURZA::Precache(void)
{
	PRECACHE_MODEL("models/v_gurza.mdl");
	PRECACHE_MODEL("models/w_gurza.mdl");
	PRECACHE_MODEL("models/p_gurza.mdl");
	PRECACHE_MODEL("models/shell9x21.mdl"); // brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gurza3.wav"); //handgun
}
