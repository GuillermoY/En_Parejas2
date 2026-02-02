// This file is part of the course TPV2@UCM - Samir Genaim

#include "MovePaddleWithKeyBoard.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"

MovePaddleWithKeyBoard::MovePaddleWithKeyBoard() :
		_up(SDL_SCANCODE_UP), //
		_down(SDL_SCANCODE_DOWN), //
		_stop(SDL_SCANCODE_LEFT), //
		_speed(10.0f) //
{
}

MovePaddleWithKeyBoard::~MovePaddleWithKeyBoard() {
}

void MovePaddleWithKeyBoard::handleInput(Container *o) {
	auto &ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(_up)) {
			o->getVel().setY(-_speed);
		} else if (ihdlr.isKeyDown(_down)) {
			o->getVel().setY(_speed);
		} else if (ihdlr.isKeyDown(_stop)) {
			o->getVel().setY(0.0f);
		}
	}
}
