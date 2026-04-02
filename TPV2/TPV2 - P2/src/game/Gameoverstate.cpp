// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameOverState.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

void GameOverState::update() {
	sdlutils().clearRenderer();

	auto& msg = sdlutils().msgs().at(_won ? "game_won" : "game_lost");
	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		(sdlutils().height() - msg.height()) / 2
	);

	sdlutils().presentRenderer();

	if (ih().keyDownEvent()) {
		Game::instance()->setState(Game::instance()->getNewGameState());
	}
}