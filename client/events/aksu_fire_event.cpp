/*
ionization weapons by kpe0
*/

#include "aksu_fire_event.h"
#include "game_event_utils.h"
#include "hud.h"
#include "const.h"
#include "utils.h"
#include "event_api.h"
#include "event_args.h"
#include "weapons/aksu.h"

CAKSUFireEvent::CAKSUFireEvent(event_args_t *args) :
	CBaseGameEvent(args)
{
}

void CAKSUFireEvent::Execute(bool secondary)
{
	if (!secondary)
		HandleShot();
	else
		HandleGrenadeLaunch();
}

void CAKSUFireEvent::HandleShot()
{
	if (IsEventLocal())
	{
		GameEventUtils::SpawnMuzzleflash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(AKSU_FIRE1 + gEngfuncs.pfnRandomLong(0, 2), 2);
	}

	matrix3x3 cameraMatrix(GetAngles());
	Vector up = cameraMatrix.GetUp();
	Vector right = cameraMatrix.GetRight();
	Vector forward = cameraMatrix.GetForward();
	int brassModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell545x39.mdl");
	Vector shellVelocity = GetVelocity() - right * gEngfuncs.pfnRandomFloat(50, 70) + up * gEngfuncs.pfnRandomFloat(100, 150) + forward * 25.0f;
	Vector shellOrigin = GetOrigin() + up * -12.0f + forward * 20.0f + right * 4.0f;

	GameEventUtils::EjectBrass(shellOrigin, GetAngles(), shellVelocity, brassModelIndex, TE_BOUNCE_SHELL);
	GameEventUtils::FireBullet(m_arguments->entindex, cameraMatrix, GetOrigin(), GetShootDirection(cameraMatrix), 2);

	const char *soundName = gEngfuncs.pfnRandomLong(0, 1) == 0 ? "weapons/aksu1.wav" : "weapons/aksu2.wav";
	gEngfuncs.pEventAPI->EV_PlaySound(GetEntityIndex(), GetOrigin(), CHAN_WEAPON, soundName, 1.f, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 15));
}

void CAKSUFireEvent::HandleGrenadeLaunch()
{
	if (IsEventLocal()) {
		gEngfuncs.pEventAPI->EV_WeaponAnimation(AKSU_LAUNCH, 2);
	}

	const char *soundName = gEngfuncs.pfnRandomLong(0, 1) == 0 ? "weapons/glauncher.wav" : "weapons/glauncher2.wav";
	gEngfuncs.pEventAPI->EV_PlaySound(GetEntityIndex(), GetOrigin(), CHAN_WEAPON, soundName, 1.f, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 15));
}

Vector CAKSUFireEvent::GetShootDirection(const matrix3x3 &camera) const
{
	return camera.GetForward() + m_arguments->fparam1 * camera.GetRight() + m_arguments->fparam2 * camera.GetUp();
}
