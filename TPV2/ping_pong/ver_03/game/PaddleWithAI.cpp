// This file is part of the course TPV2@UCM - Samir Genaim

#include "PaddleWithAI.h"
#include "../sdlutils/SDLUtils.h"

PaddleWithAI::PaddleWithAI(SDL_Color color, GameObject *ball) :
		Paddle(color), //
		_ball(ball) {
}

PaddleWithAI::~PaddleWithAI() {
}

void PaddleWithAI::handleInput() {
}

void PaddleWithAI::update() {

	if (ballComingTowardsPaddle()) {
		float diff = (_ball->getPos().getY() + _ball->getHeight() / 2.0f)
				- (_pos.getY() + _height / 2.0f);
		if (abs(diff) < 10) {
			_vel.setY(0.0f);
		} else {
			_vel.setY(
					(diff > 0.0f ? 1.0f : -1.0f) * 1.0f
							* _ball->getVel().magnitude());
		}
	} else {
		float diff = (sdlutils().height() / 2.0f)
				- (_pos.getY() + _height / 2.0f);
		if (abs(diff) < 10) {
			_vel.setY(0.0);
		} else {
			_vel.setY((diff > 0 ? 1 : -1) * 5.0f);
		}
	}

	Paddle::update();
}

bool PaddleWithAI::ballComingTowardsPaddle() {
	return (_pos.getX() < _ball->getPos().getX() && _ball->getVel().getX() < 0)
			|| (_pos.getX() > _ball->getPos().getX()
					&& _ball->getVel().getX() > 0);
}
