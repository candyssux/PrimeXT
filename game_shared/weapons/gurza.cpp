/***
*
*	ionization weapons by kpe0
*
****/

#include "gurza.h"
#include <utility>

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

CGURZAWeaponContext::CGURZAWeaponContext(std::unique_ptr<IWeaponLayer> &&layer) :
	CBaseWeaponContext(std::move(layer))
{
	m_iDefaultAmmo = GURZA_DEFAULT_GIVE;
	m_iId = WEAPON_GURZA;
	m_usFireGURZA1 = m_pLayer->PrecacheEvent("events/gurza1.sc");
	m_usFireGURZA2 = m_pLayer->PrecacheEvent("events/gurza2.sc");
}

int CGURZAWeaponContext::GetItemInfo(ItemInfo *p) const
{
	p->pszName = CLASSNAME_STR(GURZA_CLASSNAME);
	p->pszAmmo1 = "9X21";
	p->iMaxAmmo1 = _9X21_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = GURZA_MAX_CLIP;
	p->iSlot = 1;
	p->iPosition = 3;
	p->iFlags = 0;
	p->iId = m_iId;
	p->iWeight = GURZA_WEIGHT;
	return 1;
}

bool CGURZAWeaponContext::Deploy()
{
	// pev->body = 1;
	return DefaultDeploy("models/v_gurza.mdl", "models/p_gurza.mdl", GURZA_DRAW, "onehanded");
}

//void CGURZAWeaponContext::SecondaryAttack(void)
//{
//	GURZAFire(0.1, 0.2, FALSE);
//}

void CGURZAWeaponContext::PrimaryAttack(void)
{
	GURZAFire(0.03, 0.2, TRUE);
}

void CGURZAWeaponContext::GURZAFire(float flSpread, float flCycleTime, bool fUseAutoAim)
{
	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.2f);
		}

		return;
	}

	m_iClip--;

	SendWeaponAnim(m_iClip != 0 ? GURZA_SHOOT : GURZA_SHOOT_EMPTY);

#ifndef CLIENT_DLL
	// player "shoot" animation
	CBasePlayer *player = m_pLayer->GetWeaponEntity()->m_pPlayer;

	player->SetAnimation(PLAYER_ATTACK1);
	player->pev->effects = (int)(player->pev->effects) | EF_MUZZLEFLASH;

	// silenced
	if (m_pLayer->GetWeaponBodygroup() == 1)
	{
		player->m_iWeaponVolume = QUIET_GUN_VOLUME;
		player->m_iWeaponFlash = DIM_GUN_FLASH;
	}
	else
	{
		// non-silenced
		player->m_iWeaponVolume = NORMAL_GUN_VOLUME;
		player->m_iWeaponFlash = NORMAL_GUN_FLASH;
	}
#endif

	Vector vecSrc = m_pLayer->GetGunPosition();
	matrix3x3 aimMatrix = m_pLayer->GetCameraOrientation();

	if (fUseAutoAim) {
		aimMatrix.SetForward(m_pLayer->GetAutoaimVector(AUTOAIM_10DEGREES));
	}

	Vector vecDir = m_pLayer->FireBullets(1, vecSrc, aimMatrix, 8192, flSpread, BULLET_PLAYER_GURZA, m_pLayer->GetRandomSeed());
	m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(flCycleTime);
	m_flNextSecondaryAttack = m_pLayer->GetWeaponTimeBase(UsePredicting()) + flCycleTime;

	WeaponEventParams params;
	params.flags = WeaponEventFlags::NotHost;
	params.eventindex = fUseAutoAim ? m_usFireGURZA1 : m_usFireGURZA2;
	params.delay = 0.0f;
	params.origin = vecSrc;
	params.angles = aimMatrix.GetAngles();
	params.fparam1 = vecDir.x;
	params.fparam2 = vecDir.y;
	params.iparam1 = 0;
	params.iparam2 = 0;
	params.bparam1 = (m_iClip == 0) ? 1 : 0;
	params.bparam2 = 0;

	if (m_pLayer->ShouldRunFuncs()) {
		m_pLayer->PlaybackWeaponEvent(params);
	}

	m_pLayer->AddPlayerPunchangle(-2.f, 0.f, 0.f);

#ifndef CLIENT_DLL
	if (!m_iClip && m_pLayer->GetPlayerAmmo(m_iPrimaryAmmoType) <= 0)
		// HEV suit - indicate out of ammo condition
		m_pLayer->GetWeaponEntity()->m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.f, 15.f);
}

void CGURZAWeaponContext::Reload(void)
{
	int iResult;

	if (m_iClip == 0)
		iResult = DefaultReload(18, GURZA_RELOAD, 1.5);
	else
		iResult = DefaultReload(19, GURZA_RELOAD_NOT_EMPTY, 1.5);

	if (iResult)
	{
		m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.0f, 15.0f);
	}
}

void CGURZAWeaponContext::WeaponIdle(void)
{
	ResetEmptySound();

	m_pLayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > m_pLayer->GetWeaponTimeBase(UsePredicting()))
		return;

	// only idle if the slid isn't back
	if (m_iClip != 0)
	{
		int iAnim;
		float flRand = m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 0.0f, 1.0f);
		if (flRand <= 0.3 + 0 * 0.75)
		{
			iAnim = GURZA_IDLE3;
			m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 49.0 / 16;
		}
		else if (flRand <= 0.6 + 0 * 0.875)
		{
			iAnim = GURZA_IDLE1;
			m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 60.0 / 16.0;
		}
		else
		{
			iAnim = GURZA_IDLE2;
			m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 40.0 / 16.0;
		}
		SendWeaponAnim(iAnim);
	}
}
