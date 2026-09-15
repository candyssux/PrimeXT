/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>

#define WEAPON_GRACH			19
#define GRACH_WEIGHT			12
#define GRACH_MAX_CLIP			17
#define GRACH_DEFAULT_GIVE		10
#define GRACH_CLASSNAME		weapon_GRACH

enum GRACH_e
{
	GRACH_IDLE1 = 0,
	GRACH_IDLE2,
	GRACH_IDLE3,
	GRACH_SHOOT,
	GRACH_SHOOT_EMPTY,
	GRACH_RELOAD,
	GRACH_RELOAD_NOT_EMPTY,
	GRACH_DRAW,
	GRACH_HOLSTER,
	GRACH_ADD_SILENCER
};

class CGRACHWeaponContext : public CBaseWeaponContext
{
public:
	CGRACHWeaponContext() = delete;
	CGRACHWeaponContext(std::unique_ptr<IWeaponLayer> &&layer);
	~CGRACHWeaponContext() = default;

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	//void SecondaryAttack() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;
	void GRACHFire(float flSpread, float flCycleTime, bool fUseAutoAim);

	uint16_t m_usFireGRACH1;
	uint16_t m_usFireGRACH2;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CGRACHWeaponContext> {
	static constexpr int32_t value = WEAPON_GRACH;
};


