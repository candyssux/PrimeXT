/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>

#define WEAPON_GURZA			18
#define GURZA_WEIGHT			10
#define GURZA_MAX_CLIP			18
#define GURZA_DEFAULT_GIVE		10
#define GURZA_CLASSNAME		weapon_GURZA

enum GURZA_e
{
	GURZA_IDLE1 = 0,
	GURZA_IDLE2,
	GURZA_IDLE3,
	GURZA_SHOOT,
	GURZA_SHOOT_EMPTY,
	GURZA_RELOAD,
	GURZA_RELOAD_NOT_EMPTY,
	GURZA_DRAW,
	GURZA_HOLSTER,
	GURZA_ADD_SILENCER
};

class CGURZAWeaponContext : public CBaseWeaponContext
{
public:
	CGURZAWeaponContext() = delete;
	CGURZAWeaponContext(std::unique_ptr<IWeaponLayer> &&layer);
	~CGURZAWeaponContext() = default;

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	//void SecondaryAttack() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;
	void GURZAFire(float flSpread, float flCycleTime, bool fUseAutoAim);

	uint16_t m_usFireGURZA1;
	uint16_t m_usFireGURZA2;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CGURZAWeaponContext> {
	static constexpr int32_t value = WEAPON_GURZA;
};
