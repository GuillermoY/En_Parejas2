// This file is part of the course TPV2@UCM - Samir Genaim

#include "Ball.h"

#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

Ball::Ball() :
		Ball(build_sdlcolor(0xffffffff)) //
{
}

Ball::Ball(SDL_Color color) :
		_color(color) //
{
}

Ball::~Ball() {
}

void Ball::update() {
	_pos = _pos + _vel;

	// bounce on top/bottom borders
	if (_pos.getY() < 0) {
		_pos.setY(0.0f);
		_vel.setY(-_vel.getY());
		sdlutils().soundEffects().at("wall_hit").play();
	} else if (_pos.getY() + _height > sdlutils().height()) {
		_pos.setY(sdlutils().height() - _height);
		_vel.setY(-_vel.getY());
		sdlutils().soundEffects().at("wall_hit").play();
	}



}

void Ball::render() {
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(_color));

	SDL_FRect rect = build_sdlfrect(_pos.getX(), _pos.getY(), _width, _height);

	SDL_RenderFillRect(sdlutils().renderer(), &rect);
}


void Ball::handleInput() {
}
