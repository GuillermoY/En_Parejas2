// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameOverState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Health.h"
#include "Game.h"

GameOverState::GameOverState() :
	_msgKey("gameover_lose") {
}

GameOverState::~GameOverState() {
}

void GameOverState::enter() {
	// Si no quedan asteroides, el jugador ganó; si quedan, perdió
	auto* mngr = Game::Instance()->getMngr();
	bool won = mngr->getEntities(ecs::grp::ASTEROIDS).empty();
	_msgKey = won ? "gameover_win" : "gameover_lose";
}

void GameOverState::leave() {
}

void GameOverState::update() {
	// Mostrar vidas
	auto* fighter = Game::Instance()->getMngr()->getHandler(ecs::hdlr::FIGHTER);
	fighter->getComponent<Health>()->render();

	// Mostrar mensaje
	auto& t = sdlutils().msgs().at(_msgKey);
	t.render(
		(sdlutils().width() - t.width()) / 2.0f,
		(sdlutils().height() - t.height()) / 2.0f
	);

	// Cualquier tecla vuelve a NewGame
	if (ih().keyDownEvent()) {
		Game::Instance()->setState(Game::NEWGAME);
	}
}