/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <utility>

#define WEAPON_AKSU			41
#define AKSU_WEIGHT			14
#define AKSU_MAX_CLIP		30
#define AKSU_DEFAULT_AMMO	30
#define AKSU_DEFAULT_GIVE	15
#define AKSU_CLASSNAME		weapon_AKSU

enum AKSU_e
{
	AKSU_LONGIDLE = 0,
	AKSU_IDLE1,
	AKSU_LAUNCH,
	AKSU_RELOAD,
	AKSU_DEPLOY,
	AKSU_FIRE1,
	AKSU_FIRE2,
	AKSU_FIRE3,
};

class CAKSUWeaponContext : public CBaseWeaponContext
{
public:
	CAKSUWeaponContext() = delete;
	~CAKSUWeaponContext() = default;
	CAKSUWeaponContext(std::unique_ptr<IWeaponLayer> &&layer);

	int iItemSlot() override { return 3; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	//void SecondaryAttack() override;
	int SecondaryAmmoIndex() override;
	bool Deploy() override;
	void Reload() override;
	void WeaponIdle() override;

	uint16_t m_usEvent1;
	uint16_t m_usEvent2;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CAKSUWeaponContext> {
	static constexpr int32_t value = WEAPON_AKSU;
};
