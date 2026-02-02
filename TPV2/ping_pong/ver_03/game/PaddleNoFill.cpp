// This file is part of the course TPV2@UCM - Samir Genaim

#include "PaddleNoFill.h"

#include "../sdlutils/SDLUtils.h"

PaddleNoFill::PaddleNoFill(SDL_Color color) :
		Paddle(color) {
}

PaddleNoFill::~PaddleNoFill() {
}

void PaddleNoFill::render() {
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(_color));

	SDL_FRect rect = build_sdlfrect(_pos.getX(), _pos.getY(), _width, _height);

	SDL_RenderRect(sdlutils().renderer(), &rect);
}
