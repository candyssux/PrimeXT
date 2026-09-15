/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>

#define WEAPON_APS			17
#define APS_WEIGHT			11
#define APS_MAX_CLIP			20
#define APS_DEFAULT_GIVE		10
#define APS_CLASSNAME		weapon_APS

enum APS_e
{
	APS_IDLE1 = 0,
	APS_IDLE2,
	APS_IDLE3,
	APS_SHOOT,
	APS_SHOOT_EMPTY,
	APS_RELOAD,
	APS_RELOAD_NOT_EMPTY,
	APS_DRAW,
	APS_HOLSTER,
	APS_ADD_SILENCER
};

class CAPSWeaponContext : public CBaseWeaponContext
{
public:
	CAPSWeaponContext() = delete;
	CAPSWeaponContext(std::unique_ptr<IWeaponLayer> &&layer);
	~CAPSWeaponContext() = default;

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;
	void APSFire(float flSpread, float flCycleTime, bool fUseAutoAim);

	uint16_t m_usFireAPS1;
	uint16_t m_usFireAPS2;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CAPSWeaponContext> {
	static constexpr int32_t value = WEAPON_APS;
};
