/*
ionization weapons by kpe0
*/

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class COC33FireEvent : public CBaseGameEvent
{
public:
	COC33FireEvent(event_args_t *args);
	~COC33FireEvent() = default;

	void Execute();

private:
	bool ClipEmpty() const;
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
