// This file is part of the course TPV2@UCM - Samir Genaim

#include "Newgamestate.h"
#include "Game.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "messages_defs.h"

void NewGameState::enter() {
	sdlutils().soundEffects().at("pacman_intro").play("se");
}

void NewGameState::update() {
	sdlutils().clearRenderer();

	auto& msg = sdlutils().msgs().at("new_game");
	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		(sdlutils().height() - msg.height()) / 2
	);

	sdlutils().presentRenderer();

	if (ih().keyDownEvent()) {
		Message m;
		m.id = _m_NEW_GAME;
		_mngr->send(m);
		Game::instance()->setState(Game::instance()->getNewRoundState());
	}
}