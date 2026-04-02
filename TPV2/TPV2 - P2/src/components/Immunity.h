// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "../ecs/Component.h"

struct Immunity : public ecs::Component {

	static constexpr Uint32 IMMUNITY_DURATION = 10000;

	Immunity() : _active(false), _startTime(0) {}

	virtual ~Immunity() {}

	void activate(Uint32 currTime) {
		_active = true;
		_startTime = currTime;
	}

	void deactivate() {
		_active = false;
	}

	bool isExpired(Uint32 currTime) const {
		return _active && (currTime - _startTime >= IMMUNITY_DURATION);
	}

	bool   _active;
	Uint32 _startTime;
};