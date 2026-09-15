/***
*
*	ionization weapons by kpe0
*
****/

#pragma once
#include "weapon_context.h"
#include "weapon_layer.h"
#include <memory>
#include <utility>

#define WEAPON_R92			21
#define R92_WEIGHT			9
#define R92_MAX_CLIP			5
#define R92_DEFAULT_GIVE		5
#define R92_CLASSNAME		weapon_R92

enum R92_e
{
	R92_IDLE1 = 0,
	R92_FIDGET,
	R92_FIRE1,
	R92_RELOAD,
	R92_HOLSTER,
	R92_DRAW,
	R92_IDLE2,
	R92_IDLE3
};

class CR92WeaponContext : public CBaseWeaponContext
{
public:
	CR92WeaponContext() = delete;
	~CR92WeaponContext() = default;
	CR92WeaponContext(std::unique_ptr<IWeaponLayer> &&layer);

	int iItemSlot() override { return 2; }
	int GetItemInfo(ItemInfo *p) const override;
	void PrimaryAttack() override;
	//void SecondaryAttack() override;
	bool Deploy() override;
	void Holster() override;
	void Reload() override;
	void WeaponIdle() override;

	bool m_fInZoom;	// don't save this. 
	uint16_t m_usFireR92;
};

template<>
struct CBaseWeaponContext::AssignedWeaponID<CR92WeaponContext> {
	static constexpr int32_t value = WEAPON_R92;
};
