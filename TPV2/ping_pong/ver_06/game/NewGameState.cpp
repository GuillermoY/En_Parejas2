// This file is part of the course TPV2@UCM - Samir Genaim

#include "NewGameState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Game.h"
#include "GameManager.h"

NewGameState::NewGameState() :
		_msg(sdlutils().msgs().at("start")) {

}

NewGameState::~NewGameState() {
	// TODO Auto-generated destructor stub
}

void NewGameState::leave() {
}

void NewGameState::update() {

	auto &inhdlr = ih();

	if (inhdlr.keyDownEvent() && inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
		Game::Instance()->setState(Game::PAUSED_STATE);
	}

	sdlutils().clearRenderer();

	_msg.render((sdlutils().width() - _msg.width()) / 2,
			sdlutils().height() / 2 + _msg.height() * 2);

	sdlutils().presentRenderer();

}

void NewGameState::enter() {
	std::cout << std::endl;
	Game::Instance()->getGm()->resetScore();
}
