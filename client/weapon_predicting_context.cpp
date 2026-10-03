/*
weapon_predicting_context.cpp - part of client-side weapons predicting implementation
Copyright (C) 2025 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "weapon_predicting_context.h"
#include "client_weapon_layer_impl.h"
#include "hud.h"
#include "utils.h"
#include "weapons/glock.h"
#include "weapons/crossbow.h"
#include "weapons/python.h"
#include "weapons/mp5.h"
#include "weapons/shotgun.h"
#include "weapons/crowbar.h"
#include "weapons/tripmine.h"
#include "weapons/snark.h"
#include "weapons/hornetgun.h"
#include "weapons/handgrenade.h"
#include "weapons/satchel.h"
#include "weapons/rpg.h"
#include "weapons/egon.h"
#include "weapons/gauss.h"
#include "weapons/pm.h"
#include "weapons/aps.h"
#include "weapons/gurza.h"
#include "weapons/grach.h"
#include "weapons/oc33.h"
#include "weapons/r92.h"
/*
#include "weapons/flaregun.h"
#include "weapons/pb.h"
#include "weapons/p96.h"
#include "weapons/psm.h"
#include "weapons/oc23.h"
#include "weapons/oc27.h"
#include "weapons/22lr.h"
#include "weapons/p7.h"
#include "weapons/vp70.h"
#include "weapons/ks23.h"
#include "weapons/bekas.h"
#include "weapons/saiga.h"
#include "weapons/vepr.h"
#include "weapons/mc255.h"
#include "weapons/bizon.h"
#include "weapons/kedr.h"
#include "weapons/veresk.h"
#include "weapons/kiparis.h"
#include "weapons/gepard.h"
*/
#include "weapons/aksu.h"
/*
#include "weapons/aek.h"
#include "weapons/an94.h"
#include "weapons/vihr.h"
#include "weapons/akm.h"
#include "weapons/tkb0146.h"
#include "weapons/fal.h"
#include "weapons/g3.h"
#include "weapons/aug.h"
#include "weapons/svd.h"
#include "weapons/svu.h"
#include "weapons/vsk94.h"
#include "weapons/psg.h"
#include "weapons/pk.h"
#include "weapons/rpo.h"
#include "weapons/railgun.h"
#include "weapons/rgd5.h"
#include "weapons/f1.h"
#include "weapons/rgo.h"
*/
#include <cstring>

bool CWeaponPredictingContext::IsViewmodelHidden() const
{
	return m_playerState.hideViewmodel;
}

CWeaponPredictingContext::CWeaponPredictingContext()
{
	m_playerState.hideViewmodel = false;
}

void CWeaponPredictingContext::PostThink(local_state_t *from, local_state_t *to, usercmd_t *cmd, bool runfuncs, double time, uint32_t randomSeed)
{
	m_playerState.time = time;
	m_playerState.randomSeed = randomSeed;
	m_playerState.runfuncs = runfuncs;
	ReadPlayerState(from, to, cmd);
	ReadWeaponsState(from);

	const bool playerAlive = m_playerState.deadflag != (DEAD_DISCARDBODY + 1);
	CBaseWeaponContext *currWeapon = GetWeaponContext(from->client.m_iId);
	if (currWeapon)
	{
		if (runfuncs) {
			HandlePlayerSpawnDeath(to, currWeapon);
		}

		// Weapons are completely disabled while viewmodel is hidden.
		if (playerAlive && m_playerState.viewmodel && m_playerState.nextAttack <= 0.0f)
		{
			currWeapon->ItemPostFrame();
		}

		// Do not allow weapon switching while weapons are hidden/locked.
		if (playerAlive && cmd->weaponselect && m_playerState.viewmodel)
		{
			HandleWeaponSwitch(from, to, cmd, currWeapon);
		}
		else
		{
			to->client.m_iId = from->client.m_iId;
		}

		// check for desync between local & server-side weapon animation
		if (runfuncs && (m_playerState.activeWeaponanim != m_playerState.weaponanim))
		{
			gEngfuncs.pfnWeaponAnim(m_playerState.weaponanim, 0);
			m_playerState.activeWeaponanim = m_playerState.weaponanim;
		}
	}

	UpdatePlayerTimers(cmd);
	WritePlayerState(to);
	WriteWeaponsState(to, cmd);
}

void CWeaponPredictingContext::ReadPlayerState(const local_state_t *from, const local_state_t *to, usercmd_t *cmd)
{
	m_playerState.cached.buttons = from->playerstate.oldbuttons;
	int32_t buttonsChanged = (m_playerState.cached.buttons ^ cmd->buttons);
	m_playerState.buttonsPressed = buttonsChanged & cmd->buttons;	
	m_playerState.buttonsReleased = buttonsChanged & (~cmd->buttons);

	m_playerState.viewAngles = cmd->viewangles;
	m_playerState.buttons = cmd->buttons;
	m_playerState.flags = from->client.flags;

	m_playerState.deadflag = from->client.deadflag;
	m_playerState.waterlevel = from->client.waterlevel;
	m_playerState.maxSpeed = from->client.maxspeed;

	m_playerState.fov = from->client.fov;
	m_playerState.weaponanim = from->client.weaponanim;
	m_playerState.viewmodel = from->client.viewmodel;
	m_playerState.nextAttack = from->client.m_flNextAttack;

	// it's counterintuitive, but for some things we should take value from "to" state
	// because player movement prediction spits out calculations results in there
	// see CL_FinishPMove engine function to see list of such things.
	m_playerState.velocity = to->client.velocity;
	m_playerState.viewOffset = to->client.view_ofs;
	m_playerState.origin = to->playerstate.origin;
	m_playerState.punchAngle = to->client.punchangle;
}

void CWeaponPredictingContext::WritePlayerState(local_state_t *to)
{
	// here we need to write back values that potentially could be modified from weapons code
	to->client.viewmodel				= m_playerState.viewmodel;
	to->client.fov						= m_playerState.fov;
	to->client.weaponanim				= m_playerState.weaponanim;
	to->client.m_flNextAttack			= m_playerState.nextAttack;
	to->client.maxspeed					= m_playerState.maxSpeed;
	to->client.velocity					= m_playerState.velocity;
	to->client.punchangle				= m_playerState.punchAngle;
}

void CWeaponPredictingContext::UpdatePlayerTimers(const usercmd_t *cmd)
{
	m_playerState.nextAttack -= cmd->msec / 1000.0;
	if (m_playerState.nextAttack < -0.001)
		m_playerState.nextAttack = -0.001;
}

void CWeaponPredictingContext::UpdateWeaponTimers(CBaseWeaponContext *weapon, const usercmd_t *cmd)
{
	weapon->m_flNextPrimaryAttack		-= cmd->msec / 1000.0;
	weapon->m_flNextSecondaryAttack		-= cmd->msec / 1000.0;
	weapon->m_flTimeWeaponIdle			-= cmd->msec / 1000.0;

	if (weapon->m_flNextPrimaryAttack < -1.0)
		weapon->m_flNextPrimaryAttack = -1.0;

	if (weapon->m_flNextSecondaryAttack < -0.001)
		weapon->m_flNextSecondaryAttack = -0.001;

	if (weapon->m_flTimeWeaponIdle < -0.001)
		weapon->m_flTimeWeaponIdle = -0.001;

	if (weapon->m_iId == WEAPON_EGON)
	{
		CEgonWeaponContext *ctx = weapon->As<CEgonWeaponContext>();
		ctx->m_flAttackCooldown -= cmd->msec / 1000.0;
		if (ctx->m_flAttackCooldown < -0.001)
			ctx->m_flAttackCooldown = -0.001;
	}
	else if (weapon->m_iId == WEAPON_GAUSS)
	{
		CGaussWeaponContext *ctx = weapon->As<CGaussWeaponContext>();
		ctx->m_flNextAmmoBurn -= cmd->msec / 1000.0;
		if (ctx->m_flNextAmmoBurn < -0.001)
			ctx->m_flNextAmmoBurn = -0.001;

		ctx->m_flAmmoStartCharge -= cmd->msec / 1000.0;
		if (ctx->m_flAmmoStartCharge < -0.001)
			ctx->m_flAmmoStartCharge = -0.001;
	}
}

void CWeaponPredictingContext::ReadWeaponsState(const local_state_t *from)
{
	for (size_t i = 0; i < MAX_LOCAL_WEAPONS; i++)
	{
		CBaseWeaponContext *weapon = GetWeaponContext(i);
		const weapon_data_t &data = from->weapondata[i];
		if (weapon)
		{
			weapon->m_fInReload				= data.m_fInReload;
			weapon->m_fInSpecialReload		= data.m_fInSpecialReload;
			weapon->m_iClip					= data.m_iClip;
			weapon->m_flNextPrimaryAttack	= data.m_flNextPrimaryAttack;
			weapon->m_flNextSecondaryAttack	= data.m_flNextSecondaryAttack;
			weapon->m_flTimeWeaponIdle		= data.m_flTimeWeaponIdle;

			weapon->m_iSecondaryAmmoType = static_cast<int>(from->client.vuser3.z);
			weapon->m_iPrimaryAmmoType = static_cast<int>(from->client.vuser4.x);
			m_playerState.ammo[weapon->m_iPrimaryAmmoType] = static_cast<int>(from->client.vuser4.y);
			m_playerState.ammo[weapon->m_iSecondaryAmmoType] = static_cast<int>(from->client.vuser4.z);

			ReadWeaponSpecificData(weapon, from);
		}
	}
}

void CWeaponPredictingContext::WriteWeaponsState(local_state_t *to, const usercmd_t *cmd)
{
	for (size_t i = 0; i < MAX_LOCAL_WEAPONS; i++)
	{
		weapon_data_t &data = to->weapondata[i];
		CBaseWeaponContext *weapon = GetWeaponContext(i);
		if (!weapon)
		{
			std::memset(&data, 0x0, sizeof(weapon_data_t));
			continue;
		}
		else
		{
			UpdateWeaponTimers(weapon, cmd);
			data.m_fInReload				= weapon->m_fInReload;
			data.m_fInSpecialReload			= weapon->m_fInSpecialReload;
			data.m_iClip					= weapon->m_iClip; 
			data.m_flNextPrimaryAttack		= weapon->m_flNextPrimaryAttack;
			data.m_flNextSecondaryAttack	= weapon->m_flNextSecondaryAttack;
			data.m_flTimeWeaponIdle			= weapon->m_flTimeWeaponIdle;
			WriteWeaponSpecificData(weapon, to);
		}
	}
}

// don't forget to check that according information are being sent in UpdateClientData/GetWeaponData within server/client.cpp 
void CWeaponPredictingContext::ReadWeaponSpecificData(CBaseWeaponContext *weapon, const local_state_t *from)
{
	const weapon_data_t &data = from->weapondata[weapon->m_iId];
	if (weapon->m_iId == WEAPON_RPG)
	{
		CRpgWeaponContext *ctx = weapon->As<CRpgWeaponContext>();
		ctx->m_fSpotActive = static_cast<int>(from->client.vuser2.y);
		ctx->m_cActiveRockets = static_cast<int>(from->client.vuser2.z);
	}
	else if (weapon->m_iId == WEAPON_SATCHEL)
	{
		CSatchelWeaponContext *ctx = weapon->As<CSatchelWeaponContext>();
		ctx->m_chargeReady = data.iuser1;
	}
	else if (weapon->m_iId == WEAPON_HANDGRENADE)
	{
		CHandGrenadeWeaponContext *ctx = weapon->As<CHandGrenadeWeaponContext>();
		ctx->m_flStartThrow = data.fuser1;
		ctx->m_flReleaseThrow = data.fuser2;
	}
	else if (weapon->m_iId == WEAPON_EGON)
	{
		CEgonWeaponContext *ctx = weapon->As<CEgonWeaponContext>();
		ctx->m_flAttackCooldown = data.fuser1;
	}
	else if (weapon->m_iId == WEAPON_GAUSS)
	{
		CGaussWeaponContext *ctx = weapon->As<CGaussWeaponContext>();
		ctx->m_flAmmoStartCharge = data.fuser1;
		ctx->m_flNextAmmoBurn = data.fuser2;
		ctx->m_fInAttack = data.iuser1;
	}
}

void CWeaponPredictingContext::WriteWeaponSpecificData(CBaseWeaponContext *weapon, local_state_t *to)
{
	weapon_data_t &data = to->weapondata[weapon->m_iId];
	if (weapon->m_iId == WEAPON_RPG)
	{
		CRpgWeaponContext *ctx = weapon->As<CRpgWeaponContext>();
		to->client.vuser2.y = ctx->m_fSpotActive;
		to->client.vuser2.z = ctx->m_cActiveRockets;
	}
	else if (weapon->m_iId == WEAPON_SATCHEL)
	{
		CSatchelWeaponContext *ctx = weapon->As<CSatchelWeaponContext>();
		data.iuser1 = ctx->m_chargeReady;
	}
	else if (weapon->m_iId == WEAPON_HANDGRENADE)
	{
		CHandGrenadeWeaponContext *ctx = weapon->As<CHandGrenadeWeaponContext>();
		data.fuser1 = ctx->m_flStartThrow;
		data.fuser2 = ctx->m_flReleaseThrow;
	}
	else if (weapon->m_iId == WEAPON_EGON)
	{
		CEgonWeaponContext *ctx = weapon->As<CEgonWeaponContext>();
		data.fuser1 = ctx->m_flAttackCooldown;
	}
	else if (weapon->m_iId == WEAPON_GAUSS)
	{
		CGaussWeaponContext *ctx = weapon->As<CGaussWeaponContext>();
		data.fuser1 = ctx->m_flAmmoStartCharge;
		data.fuser2 = ctx->m_flNextAmmoBurn;
		data.iuser1 = ctx->m_fInAttack;
	}
}

void CWeaponPredictingContext::HandlePlayerSpawnDeath(local_state_t *to, CBaseWeaponContext *weapon)
{
	if (to->client.health <= 0 && m_playerState.cached.health > 0) {
		weapon->Holster();
	}
	else if (to->client.health > 0 && m_playerState.cached.health <= 0) {
		weapon->Deploy();
	}
	m_playerState.cached.health = to->client.health;
}

void CWeaponPredictingContext::HandleWeaponSwitch(const local_state_t *from, local_state_t *to, const usercmd_t *cmd, CBaseWeaponContext *weapon)
{
	if (from->weapondata[cmd->weaponselect].m_iId == cmd->weaponselect)
	{
		CBaseWeaponContext *selectedWeapon = GetWeaponContext(cmd->weaponselect);
		if (selectedWeapon && selectedWeapon->m_iId != weapon->m_iId)
		{
			weapon->Holster();
			selectedWeapon->Deploy();
			to->client.m_iId = cmd->weaponselect;
		}
	}
}

CBaseWeaponContext* CWeaponPredictingContext::GetWeaponContext(uint32_t weaponID)
{
	if (m_weaponsState.count(weaponID)) {
		return m_weaponsState[weaponID].get();
	}
	else
	{
		switch (weaponID)
		{
			case WEAPON_GLOCK:  
				m_weaponsState[weaponID] = std::make_unique<CGlockWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_CROSSBOW:
				m_weaponsState[weaponID] = std::make_unique<CCrossbowWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PYTHON:
				m_weaponsState[weaponID] = std::make_unique<CPythonWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_MP5:
				m_weaponsState[weaponID] = std::make_unique<CMP5WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SHOTGUN:
				m_weaponsState[weaponID] = std::make_unique<CShotgunWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_CROWBAR:
				m_weaponsState[weaponID] = std::make_unique<CCrowbarWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_TRIPMINE:
				m_weaponsState[weaponID] = std::make_unique<CTripmineWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SNARK:
				m_weaponsState[weaponID] = std::make_unique<CSqueakWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_HORNETGUN:
				m_weaponsState[weaponID] = std::make_unique<CHornetgunWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_HANDGRENADE:
				m_weaponsState[weaponID] = std::make_unique<CHandGrenadeWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SATCHEL:
				m_weaponsState[weaponID] = std::make_unique<CSatchelWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_RPG:
				m_weaponsState[weaponID] = std::make_unique<CRpgWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_EGON:
				m_weaponsState[weaponID] = std::make_unique<CEgonWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_GAUSS:
				m_weaponsState[weaponID] = std::make_unique<CGaussWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PM:
				m_weaponsState[weaponID] = std::make_unique<CPMWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_APS:
				m_weaponsState[weaponID] = std::make_unique<CAPSWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_GURZA:
				m_weaponsState[weaponID] = std::make_unique<CGURZAWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_GRACH:
				m_weaponsState[weaponID] = std::make_unique<CGRACHWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_OC33:
				m_weaponsState[weaponID] = std::make_unique<COC33WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_R92:
				m_weaponsState[weaponID] = std::make_unique<CR92WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
				/*
			case WEAPON_FLAREGUN:
				m_weaponsState[weaponID] = std::make_unique<CFLAREGUNWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PB:
				m_weaponsState[weaponID] = std::make_unique<CPBWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_P96:
				m_weaponsState[weaponID] = std::make_unique<CP96WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PSM:
				m_weaponsState[weaponID] = std::make_unique<CPSMWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_OC23:
				m_weaponsState[weaponID] = std::make_unique<COC23WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_OC27:
				m_weaponsState[weaponID] = std::make_unique<COC27WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_22LR:
				m_weaponsState[weaponID] = std::make_unique<C22LRWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_P7:
				m_weaponsState[weaponID] = std::make_unique<CP7WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_VP70:
				m_weaponsState[weaponID] = std::make_unique<CVP70WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_KS23:
				m_weaponsState[weaponID] = std::make_unique<CKS23WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_BEKAS:
				m_weaponsState[weaponID] = std::make_unique<CBEKASWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SAIGA:
				m_weaponsState[weaponID] = std::make_unique<CSAIGAWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_VEPR:
				m_weaponsState[weaponID] = std::make_unique<CVEPRWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_MC255:
				m_weaponsState[weaponID] = std::make_unique<CMC255WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_BIZON:
				m_weaponsState[weaponID] = std::make_unique<CBIZONWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_KEDR:
				m_weaponsState[weaponID] = std::make_unique<CKEDRWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_VERESK:
				m_weaponsState[weaponID] = std::make_unique<CVERESKWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_KIPARIS:
				m_weaponsState[weaponID] = std::make_unique<CKIPARISWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_GEPARD:
				m_weaponsState[weaponID] = std::make_unique<CGEPARDWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
				*/
			case WEAPON_AKSU:
				m_weaponsState[weaponID] = std::make_unique<CAKSUWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
				/*
			case WEAPON_AEK:
				m_weaponsState[weaponID] = std::make_unique<CAEKWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_AN94:
				m_weaponsState[weaponID] = std::make_unique<CAN94WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_VIHR:
				m_weaponsState[weaponID] = std::make_unique<CVIHRWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_AKM:
				m_weaponsState[weaponID] = std::make_unique<CAKMWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_TKB0146:
				m_weaponsState[weaponID] = std::make_unique<CTKB0146WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_FAL:
				m_weaponsState[weaponID] = std::make_unique<CFALWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_G3:
				m_weaponsState[weaponID] = std::make_unique<CG3WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_AUG:
				m_weaponsState[weaponID] = std::make_unique<CAUGWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SVD:
				m_weaponsState[weaponID] = std::make_unique<CSVDWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_SVU:
				m_weaponsState[weaponID] = std::make_unique<CSVUWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_VSK94:
				m_weaponsState[weaponID] = std::make_unique<CVSK94WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PSG:
				m_weaponsState[weaponID] = std::make_unique<CPSGWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_PK:
				m_weaponsState[weaponID] = std::make_unique<CPKWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_RPO:
				m_weaponsState[weaponID] = std::make_unique<CRPOWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_RAILGUN:
				m_weaponsState[weaponID] = std::make_unique<CRAILGUNWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_RGD5:
				m_weaponsState[weaponID] = std::make_unique<CRGD5WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_F1:
				m_weaponsState[weaponID] = std::make_unique<CF1WeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
			case WEAPON_RGO:
				m_weaponsState[weaponID] = std::make_unique<CRGOWeaponContext>(std::make_unique<CClientWeaponLayerImpl>(m_playerState));
				break;
				*/
			default: 
				return nullptr;
		}

		ItemInfo itemInfo;
		m_weaponsState[weaponID]->GetItemInfo(&itemInfo);
		CBaseWeaponContext::ItemInfoArray[weaponID] = itemInfo;
		return m_weaponsState[weaponID].get();
	}
}
