// This file is part of the course TPV2@UCM - Samir Genaim

#include "PaddleWithMouse.h"
#include "../sdlutils/InputHandler.h"

PaddleWithMouse::PaddleWithMouse(SDL_Color color) :
		Paddle(color) {
}

PaddleWithMouse::~PaddleWithMouse() {
}

void PaddleWithMouse::handleInput() {
	if (ih().mouseMotionEvent())
		_pos.setY(ih().getMousePos().second);
}

