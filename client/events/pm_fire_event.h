/*
ionization weapons by kpe0
*/

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class CPMFireEvent : public CBaseGameEvent
{
public:
	CPMFireEvent(event_args_t *args);
	~CPMFireEvent() = default;

	void Execute();

private:
	bool ClipEmpty() const;
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
