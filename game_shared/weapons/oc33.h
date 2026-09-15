/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>

#define WEAPON_OC33			20
#define OC33_WEIGHT			11
#define OC33_MAX_CLIP			18
#define OC33_DEFAULT_GIVE		10
#define OC33_CLASSNAME		weapon_OC33

enum OC33_e
{
	OC33_IDLE1 = 0,
	OC33_IDLE2,
	OC33_IDLE3,
	OC33_SHOOT,
	OC33_SHOOT_EMPTY,
	OC33_RELOAD,
	OC33_RELOAD_NOT_EMPTY,
	OC33_DRAW,
	OC33_HOLSTER,
	OC33_ADD_SILENCER
};

class COC33WeaponContext : public CBaseWeaponContext
{
public:
	COC33WeaponContext() = delete;
	COC33WeaponContext(std::unique_ptr<IWeaponLayer> &&layer);
	~COC33WeaponContext() = default;

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;
	void OC33Fire(float flSpread, float flCycleTime, bool fUseAutoAim);

	uint16_t m_usFireOC331;
	uint16_t m_usFireOC332;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<COC33WeaponContext> {
	static constexpr int32_t value = WEAPON_OC33;
};
