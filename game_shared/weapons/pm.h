/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>

#define WEAPON_PM			16
#define PM_WEIGHT			10
#define PM_MAX_CLIP			8
#define PM_DEFAULT_GIVE		8
#define PM_CLASSNAME		weapon_PM

enum PM_e
{
	PM_IDLE1 = 0,
	PM_IDLE2,
	PM_IDLE3,
	PM_SHOOT,
	PM_SHOOT_EMPTY,
	PM_RELOAD,
	PM_RELOAD_NOT_EMPTY,
	PM_DRAW,
	PM_HOLSTER,
	PM_ADD_SILENCER
};

class CPMWeaponContext : public CBaseWeaponContext
{
public:
	CPMWeaponContext() = delete;
	CPMWeaponContext(std::unique_ptr<IWeaponLayer> &&layer);
	~CPMWeaponContext() = default;

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	//void SecondaryAttack() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;
	void PMFire(float flSpread, float flCycleTime, bool fUseAutoAim);

	uint16_t m_usFirePM1;
	uint16_t m_usFirePM2;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CPMWeaponContext> {
	static constexpr int32_t value = WEAPON_PM;
};
