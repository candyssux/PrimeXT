/***
*
*	ionization weapons by kpe0
*
****/

#include "weapon_pm.h"
#include "weapon_layer.h"
#include "weapons/pm.h"
#include "server_weapon_layer_impl.h"

LINK_ENTITY_TO_CLASS(weapon_pm, CPM);

CPM::CPM()
{
	auto layerImpl = std::make_unique<CServerWeaponLayerImpl>(this);
	auto contextImpl = std::make_unique<CPMWeaponContext>(std::move(layerImpl));
	m_pWeaponContext = std::move(contextImpl);
}

void CPM::Spawn()
{
	pev->classname = MAKE_STRING(CLASSNAME_STR(PM_CLASSNAME)); // hack to allow for old names
	Precache();
	SET_MODEL(edict(), "models/w_pm.mdl");
	FallInit();// get ready to fall down.
}

void CPM::Precache(void)
{
	PRECACHE_MODEL("models/v_pm.mdl");
	PRECACHE_MODEL("models/w_pm.mdl");
	PRECACHE_MODEL("models/p_pm.mdl");
	PRECACHE_MODEL("models/shell.mdl"); // brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav"); //silenced handgun
	PRECACHE_SOUND("weapons/pl_pm3.wav"); //handgun
}
