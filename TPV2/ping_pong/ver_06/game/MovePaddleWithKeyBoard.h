// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "InputComponent.h"

#include <SDL.h>

class MovePaddleWithKeyBoard: public InputComponent {
public:
	MovePaddleWithKeyBoard();
	virtual ~MovePaddleWithKeyBoard();

	inline void setKeys(SDL_Scancode up, SDL_Scancode down, SDL_Scancode stop) {
		_up = up;
		_down = down;
		_stop = stop;
	}

	inline void setSpeed(float speed) {
		_speed = speed;
	}

	void handleInput(Container *o) override;

private:
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _stop;
	float _speed;
};


