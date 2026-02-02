// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameOverState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"
#include "GameManager.h"

GameOverState::GameOverState() :
		_msg(sdlutils().msgs().at("gameover")) {

}

GameOverState::~GameOverState() {
	// TODO Auto-generated destructor stub
}

void GameOverState::leave() {
}

void GameOverState::update() {
	auto &inhdlr = ih();

	if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
		Game::Instance()->setState(Game::NEWGAME_STATE);
	}

	sdlutils().clearRenderer();

	Game::Instance()->getGm()->render();

	_msg.render((sdlutils().width() - _msg.width()) / 2,
			sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();
}

void GameOverState::enter() {
}
