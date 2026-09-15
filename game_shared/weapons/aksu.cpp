/*
*
*	ionization weapons by kpe0
*
*/

#include "aksu.h"

#ifdef CLIENT_DLL
#else
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "ggrenade.h"
#endif

CAKSUWeaponContext::CAKSUWeaponContext(std::unique_ptr<IWeaponLayer> &&layer) :
	CBaseWeaponContext(std::move(layer))
{
	m_iId = WEAPON_AKSU;
	m_iDefaultAmmo = AKSU_DEFAULT_GIVE;
	m_usEvent1 = m_pLayer->PrecacheEvent("events/aksu.sc");
	m_usEvent2 = m_pLayer->PrecacheEvent("events/aksu2.sc");
}

int CAKSUWeaponContext::GetItemInfo(ItemInfo *p) const
{
	p->pszName = CLASSNAME_STR(AKSU_CLASSNAME);
	p->pszAmmo1 = "545X39";
	p->iMaxAmmo1 = _545X39_MAX_CARRY;
	//p->pszAmmo2 = "ARgrenades";
	//p->iMaxAmmo2 = M203_GRENADE_MAX_CARRY;
	p->iMaxClip = AKSU_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 0;
	p->iFlags = 0;
	p->iId = m_iId;
	p->iWeight = AKSU_WEIGHT;
	return 1;
}

int CAKSUWeaponContext::SecondaryAmmoIndex()
{
	return m_iSecondaryAmmoType;
}

bool CAKSUWeaponContext::Deploy()
{
	return DefaultDeploy("models/v_aksu.mdl", "models/p_aksu.mdl", AKSU_DEPLOY, "aksu");
}

void CAKSUWeaponContext::PrimaryAttack()
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
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.15f);
		return;
	}

	m_iClip--;

	Vector vecSrc = m_pLayer->GetGunPosition();
	matrix3x3 cameraTransform = m_pLayer->GetCameraOrientation();
	cameraTransform.SetForward(m_pLayer->GetAutoaimVector(AUTOAIM_5DEGREES));
	Vector spread = m_pLayer->IsMultiplayer() ? VECTOR_CONE_5DEGREES : VECTOR_CONE_5DEGREES;
	Vector vecDir = m_pLayer->FireBullets(1, vecSrc, cameraTransform, 8192, spread.x, BULLET_PLAYER_AKSU, m_pLayer->GetRandomSeed());

	WeaponEventParams params;
	params.flags = WeaponEventFlags::NotHost;
	params.eventindex = m_usEvent1;
	params.delay = 0.0f;
	params.origin = vecSrc;
	params.angles = cameraTransform.GetAngles();
	params.fparam1 = vecDir.x;
	params.fparam2 = vecDir.y;
	params.iparam1 = 0;
	params.iparam2 = 0;
	params.bparam1 = 0;
	params.bparam2 = 0;

	if (m_pLayer->ShouldRunFuncs()) {
		m_pLayer->PlaybackWeaponEvent(params);
	}

	m_pLayer->AddPlayerPunchangle(m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), -4.f, 4.f), -4.f, 4.f);

#ifndef CLIENT_DLL
	CBasePlayer *player = m_pLayer->GetWeaponEntity()->m_pPlayer;
	player->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	player->m_iWeaponFlash = NORMAL_GUN_FLASH;
	player->pev->effects = (int)(player->pev->effects) | EF_MUZZLEFLASH;
	player->SetAnimation(PLAYER_ATTACK1);

	if (!m_iClip && player->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		player->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif

	m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.1f);
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.f, 15.f);
}

/*void CAKSUWeaponContext::SecondaryAttack()
{
	// don't fire underwater
	if (m_pLayer->GetPlayerWaterlevel() == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(0.15f);
		return;
	}

	if (m_pLayer->GetPlayerAmmo(m_iSecondaryAmmoType) < 1)
	{
		PlayEmptySound();
		return;
	}

	m_pLayer->SetPlayerAmmo(m_iSecondaryAmmoType, m_pLayer->GetPlayerAmmo(m_iSecondaryAmmoType) - 1);

	WeaponEventParams params;
	params.flags = WeaponEventFlags::NotHost;
	params.eventindex = m_usEvent2;
	params.delay = 0.0f;
	params.origin = m_pLayer->GetGunPosition();
	params.angles = m_pLayer->GetViewAngles();
	params.fparam1 = 0;
	params.fparam2 = 0;
	params.iparam1 = 0;
	params.iparam2 = 0;
	params.bparam1 = 0;
	params.bparam2 = 0;

	if (m_pLayer->ShouldRunFuncs()) {
		m_pLayer->PlaybackWeaponEvent(params);
	}

#ifndef CLIENT_DLL
	CBasePlayer *player = m_pLayer->GetWeaponEntity()->m_pPlayer;
	player->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	player->m_iWeaponFlash = BRIGHT_GUN_FLASH;
	player->m_iExtraSoundTypes = bits_SOUND_DANGER;
	player->m_flStopExtraSoundTime = gpGlobals->time + 0.2;
	player->SetAnimation(PLAYER_ATTACK1);

	UTIL_MakeVectors(player->pev->v_angle + player->pev->punchangle);

	// we don't add in player velocity anymore.
	CGrenade::ShootContact(player->pev, player->EyePosition() + gpGlobals->v_forward * 16, gpGlobals->v_forward * 800);

	if (!player->m_rgAmmo[m_iSecondaryAmmoType])
		// HEV suit - indicate out of ammo condition
		player->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif

	m_pLayer->AddPlayerPunchangle(-10.f, 0.f, 0.f);
	m_flNextPrimaryAttack = GetNextPrimaryAttackDelay(1.0f);
	m_flNextSecondaryAttack = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 1.f;
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + 5.f; // idle pretty soon after shooting.
}
*/
void CAKSUWeaponContext::Reload()
{
	DefaultReload(AKSU_MAX_CLIP, AKSU_RELOAD, 1.5);
}

void CAKSUWeaponContext::WeaponIdle()
{
	ResetEmptySound();
	m_pLayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	if (m_flTimeWeaponIdle > m_pLayer->GetWeaponTimeBase(UsePredicting()))
		return;

	SendWeaponAnim(m_pLayer->GetRandomInt(m_pLayer->GetRandomSeed(), 0, 1) == 0 ? AKSU_LONGIDLE : AKSU_IDLE1);
	m_flTimeWeaponIdle = m_pLayer->GetWeaponTimeBase(UsePredicting()) + m_pLayer->GetRandomFloat(m_pLayer->GetRandomSeed(), 10.f, 15.f);
}
