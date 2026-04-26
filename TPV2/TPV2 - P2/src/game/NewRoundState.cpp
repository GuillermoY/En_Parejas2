// This file is part of the course TPV2@UCM - Samir Genaim

#include "Newroundstate.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "messages_defs.h"

void NewRoundState::update() {
	sdlutils().clearRenderer();

	auto& msg = sdlutils().msgs().at("new_round");
	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		(sdlutils().height() - msg.height()) / 2
	);

	sdlutils().presentRenderer();

	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN)) {
		Message m;
		m.id = _m_ROUND_START;
		_mngr->send(m);
		Game::instance()->setState(Game::instance()->getRunningState());
	}
}