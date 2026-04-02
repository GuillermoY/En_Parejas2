// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_scancode.h>
#include <SDL_stdinc.h>

#include "../ecs/Component.h"
struct PaddleCtrl: ecs::Component {


	enum CtrlType : Uint8 {
		KEYBOARD, MOUSE, AI
	};

	PaddleCtrl() :
			_ctrlType(KEYBOARD), //
			_up(SDL_SCANCODE_UP), //
			_down(SDL_SCANCODE_DOWN), //
			_stop(SDL_SCANCODE_LEFT), //
			_speed(10.0f) //
	{
	}
	virtual ~PaddleCtrl() {
	}

	inline void setKeys(SDL_Scancode up, SDL_Scancode down, SDL_Scancode stop) {
		_up = up;
		_down = down;
		_stop = stop;
	}

	inline void setSpeed(float speed) {
		_speed = speed;
	}

	inline void setType(CtrlType ctrlType) {
		_ctrlType = ctrlType;
	}

	CtrlType _ctrlType;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _stop;
	float _speed;
};

