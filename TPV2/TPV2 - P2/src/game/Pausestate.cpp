// This file is part of the course TPV2@UCM - Samir Genaim

#include "Pausestate.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

void PauseState::update() {
	sdlutils().clearRenderer();

	auto& msg = sdlutils().msgs().at("paused");
	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		(sdlutils().height() - msg.height()) / 2
	);

	sdlutils().presentRenderer();

	if (ih().keyDownEvent()) {
		Game::instance()->setState(Game::instance()->getRunningState());
	}
}