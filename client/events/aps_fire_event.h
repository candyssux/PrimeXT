/*
ionization weapons by kpe0
*/

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class CAPSFireEvent : public CBaseGameEvent
{
public:
	CAPSFireEvent(event_args_t *args);
	~CAPSFireEvent() = default;

	void Execute();

private:
	bool ClipEmpty() const;
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
