

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class CR92FireEvent : public CBaseGameEvent
{
public:
	CR92FireEvent(event_args_t *args);
	~CR92FireEvent() = default;

	void Execute();

private:
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
