// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "../ecs/Component.h"

struct MiracleFood : public ecs::Component {

	enum State { NORMAL, MIRACULOUS };

	MiracleFood() :
		_state(NORMAL), _N(0), _M(0), _stateStartTime(0) {
	}

	virtual ~MiracleFood() {}

	void init(Uint32 N_ms, Uint32 startTime) {
		_state = NORMAL;
		_N = N_ms;
		_M = 0;
		_stateStartTime = startTime;
	}

	bool isMiraculous() const {
		return _state == MIRACULOUS;
	}

	State  _state;
	Uint32 _N;
	Uint32 _M;
	Uint32 _stateStartTime;
};