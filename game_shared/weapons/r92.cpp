/***
*
*	ionization weapons by kpe0
*
****/

#include "r92.h"

#ifdef CLIENT_DLL
#else
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#endif

// check VECTOR_CONE_1DEGREES macro
#define CONE_2DEGREES	0.01745

CR92WeaponContext::CR92WeaponContext(std::unique_ptr<IWeaponLayer>&& layer) :
	CBaseWeaponContext(std::move(layer))
{
	m_iId = WEAPON_R92;
	m_iDefaultAmmo = R92_DEFAULT_GIVE;
	m_fInZoom = false;
	m_usFireR92 = m_pLayer->PrecacheEvent("events/r92.sc");
}

int CR92WeaponContext::GetItemInfo(ItemInfo *p) const
{
	p->pszName = CLASSNAME_STR(R92_CLASSNAME);
	p->pszAmmo1 = "9X18";
	p->iMaxAmmo1 = _9X18_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = R92_MAX_CLIP;
	p->iFlags = 0;
	p->iSlot = 1;
	p->iPosition = 1;
	p->iId = m_iId;
	p->iWeight = R92_WEIGHT;
	return 1;
}

bool CR92WeaponContext::Deploy()
{
	if (m_pLayer->IsMultiplayer())
	{
		// enable laser sight geometry.
		m_pLayer->SetWeaponBodygroup(1);
	}
	else
	{
		m_pLayer->SetWeaponBodygroup(0);
	}

	return DefaultDeploy("models/v_r92.mdl", "models/p_r92.mdl", R92_DRAW, "r92");
}

void CR92WeaponContext::Holster()
{
	m_fInReload = FALSE; // cancel any reload in progress.

	if (m_fInZoom) {
		SecondaryAttack();
	}

	m_pLayer->SetPlayerNextAttackTime(m_pLayer->GetWeaponTimeBase(UsePredicting()) + 1.0f);
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.f, 15.f);
	SendWeaponAnim(R92_HOLSTER);
}

/*
void CR92WeaponContext::SecondaryAttack()
{
	if (!m_pLayer->IsMultiplayer())
	{
		return;
	}

	if (m_pLayer->GetPlayerFOV() != 0.0f)
	{
		m_pLayer->SetPlayerFOV(0.0f); // 0 means reset to default fov
		m_fInZoom = false;
	}
	else
	{
		m_pLayer->SetPlayerFOV(40.0f);
		m_fInZoom = true;
	}

	m_flNextSecondaryAttack = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 0.5f;
}
*/

void CR92WeaponContext::PrimaryAttack()
{
	// don't fire underwater
	if (m_pLayer->GetPlayerWaterlevel() == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.15f);
		return;
	}

	if (m_iClip <= 0)
	{
		if (!m_fFireOnEmpty)
		{
			Reload();
		}
		else
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.15f);
		}
		return;
	}

	m_iClip--;

	Vector vecSrc = m_pLayer->GetGunPosition();
	matrix3x3 cameraTransform = m_pLayer->GetCameraOrientation();
	cameraTransform.SetForward(m_pLayer->GetAutoaimVector(AUTOAIM_10DEGREES));
	Vector spread = m_pLayer->FireBullets(1, vecSrc, cameraTransform, 8192, CONE_2DEGREES, BULLET_PLAYER_R92, m_pLayer->GetRandomSeed());

	WeaponEventParams params;
	params.flags = WeaponEventFlags::NotHost;
	params.eventindex = m_usFireR92;
	params.delay = 0.0f;
	params.origin = vecSrc;
	params.angles = cameraTransform.GetAngles();
	params.fparam1 = spread.x;
	params.fparam2 = spread.y;
	params.iparam1 = 0;
	params.iparam2 = 0;
	params.bparam1 = (m_iClip == 0) ? 1 : 0;
	params.bparam2 = 0;

	if (m_pLayer->ShouldRunFuncs()) {
		m_pLayer->PlaybackWeaponEvent(params);
	}

	m_pLayer->AddPlayerPunchangle(-3.f, 0.f, 0.f);

#ifndef CLIENT_DLL
	CBasePlayer *player = m_pLayer->GetWeaponEntity()->m_pPlayer;

	player->SetAnimation(PLAYER_ATTACK1);
	player->pev->effects = (int)(player->pev->effects) | EF_MUZZLEFLASH;
	player->m_iWeaponVolume = LOUD_GUN_VOLUME;
	player->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	if (!m_iClip && player->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		player->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif

	m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.75f);
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.f, 15.f);
}

void CR92WeaponContext::Reload()
{
	if (m_pLayer->GetPlayerAmmo(m_iPrimaryAmmoType) < 1)
		return;

	if (m_pLayer->GetPlayerFOV() != 0.0f)
	{
		m_pLayer->SetPlayerFOV(0.0f); // 0 means reset to default fov
		m_fInZoom = false;
	}

	DefaultReload(5, R92_RELOAD, 2.0f, m_pLayer->IsMultiplayer() ? 1 : 0);
}

void CR92WeaponContext::WeaponIdle()
{
	ResetEmptySound();

	m_pLayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > m_pLayer->GetWeaponTimeBase(UsePredicting()))
		return;

	int iAnim;
	float flRand = m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 0.f, 1.f);
	if (flRand <= 0.5f)
	{
		iAnim = R92_IDLE1;
		m_flTimeWeaponIdle = (70.0 / 30.0);
	}
	else if (flRand <= 0.7f)
	{
		iAnim = R92_IDLE2;
		m_flTimeWeaponIdle = (60.0 / 30.0);
	}
	else if (flRand <= 0.9f)
	{
		iAnim = R92_IDLE3;
		m_flTimeWeaponIdle = (88.0 / 30.0);
	}
	else
	{
		iAnim = R92_FIDGET;
		m_flTimeWeaponIdle = (170.0 / 30.0);
	}

	SendWeaponAnim(iAnim, m_pLayer->IsMultiplayer() ? 1 : 0);
}
