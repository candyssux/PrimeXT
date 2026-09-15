/*
ionization weapons by kpe0
*/

#pragma once
#include "base_game_event.h"
#include "matrix.h"

class CGRACHFireEvent : public CBaseGameEvent
{
public:
	CGRACHFireEvent(event_args_t *args);
	~CGRACHFireEvent() = default;

	void Execute();

private:
	bool ClipEmpty() const;
	Vector GetShootDirection(const matrix3x3 &camera) const;
};
