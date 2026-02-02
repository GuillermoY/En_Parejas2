// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "GameObject.h"

class Paddle: public GameObject {
public:
	Paddle();
	Paddle(SDL_Color color);
	virtual ~Paddle();

	void handleInput() override;
	void update() override;
	void render() override;

	inline void setKeys(SDL_Scancode up, SDL_Scancode down, SDL_Scancode stop) {
		_up = up;
		_down = down;
		_stop = stop;
	}

	inline void setSpeed(float speed) {
		_speed = speed;
	}

private:
	SDL_Color _color;
	SDL_Scancode _up;
	SDL_Scancode _down;
	SDL_Scancode _stop;
	float _speed;
};

