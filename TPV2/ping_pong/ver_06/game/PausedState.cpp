// This file is part of the course TPV2@UCM - Samir Genaim

#include "PausedState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"
#include "GameObject.h"

PausedState::PausedState() :
		_msg(sdlutils().msgs().at("continue")) {

}

PausedState::~PausedState() {
	// TODO Auto-generated destructor stub
}

void PausedState::leave() {
}

void PausedState::update() {
	auto &inhdlr = ih();

	if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
		Game::Instance()->setState(Game::RUNNING_STATE);
	}

	sdlutils().clearRenderer();

	// render
	for (auto &o : Game::Instance()->getObjs()) {
		o->render();
	}

	_msg.render((sdlutils().width() - _msg.width()) / 2,
			sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();
}

void PausedState::enter() {
	auto _ball = Game::Instance()->getBall();
	_ball->getPos().set(sdlutils().width() / 2 - 5,
			sdlutils().height() / 2 - 5);
	_ball->getVel().set(0, 0);
}
