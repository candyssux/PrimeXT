

#include "r92_fire_event.h"
#include "game_event_utils.h"
#include "hud.h"
#include "const.h"
#include "utils.h"
#include "event_api.h"
#include "event_args.h"
#include "weapons/r92.h"

CR92FireEvent::CR92FireEvent(event_args_t *args) :
	CBaseGameEvent(args)
{
}

void CR92FireEvent::Execute()
{
	if (IsEventLocal())
	{
		GameEventUtils::SpawnMuzzleflash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(R92_FIRE1, 0);
	}

	matrix3x3 cameraMatrix(GetAngles());
	Vector up = cameraMatrix.GetUp();
	Vector right = cameraMatrix.GetRight();
	Vector forward = cameraMatrix.GetForward();
	int brassModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl");
	Vector shellVelocity = GetVelocity() + right * gEngfuncs.pfnRandomFloat(50, 70) + up * gEngfuncs.pfnRandomFloat(100, 150) + forward * 25.0f;
	Vector shellOrigin = GetOrigin() + up * -12.0f + forward * 20.0f + right * 4.0f;

	GameEventUtils::EjectBrass(shellOrigin, GetAngles(), shellVelocity, brassModelIndex, TE_BOUNCE_SHELL);
	GameEventUtils::FireBullet(m_arguments->entindex, cameraMatrix, GetOrigin(), GetShootDirection(cameraMatrix), 1);

	const char *soundName = gEngfuncs.pfnRandomLong(0, 1) == 0 ? "weapons/r92_shot1.wav" : "weapons/r92_shot2.wav";
	gEngfuncs.pEventAPI->EV_PlaySound(GetEntityIndex(), GetOrigin(), CHAN_WEAPON, soundName, gEngfuncs.pfnRandomFloat(0.8, 0.9), ATTN_NORM, 0, PITCH_NORM);
}

Vector CR92FireEvent::GetShootDirection(const matrix3x3 &camera) const
{
	return camera.GetForward() + m_arguments->fparam1 * camera.GetRight() + m_arguments->fparam2 * camera.GetUp();
}
