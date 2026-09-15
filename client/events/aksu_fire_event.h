/*
ionization weapons by kpe0
*/

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class CAKSUFireEvent : public CBaseGameEvent
{
public:
	CAKSUFireEvent(event_args_t *args);
	~CAKSUFireEvent() = default;

	void Execute(bool secondary);

private:
	void HandleShot();
	void HandleGrenadeLaunch();
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
