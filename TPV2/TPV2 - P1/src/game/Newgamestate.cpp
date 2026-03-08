// This file is part of the course TPV2@UCM - Samir Genaim

#include "NewGameState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Health.h"
#include "FighterUtils.h"
#include "Game.h"

NewGameState::NewGameState() {
}

NewGameState::~NewGameState() {
}

void NewGameState::enter() {
}

void NewGameState::leave() {
}

void NewGameState::update() {
	// Mostrar mensaje
	auto& t = sdlutils().msgs().at("new_game");
	t.render(
		(sdlutils().width() - t.width()) / 2.0f,
		(sdlutils().height() - t.height()) / 2.0f
	);

	// Mostrar vidas
	auto* fighter = Game::Instance()->getMngr()->getHandler(ecs::hdlr::FIGHTER);
	fighter->getComponent<Health>()->render();

	// Esperar tecla para continuar
	if (ih().keyDownEvent()) {
		Game::Instance()->getFighterUtils()->reset_lives();
		Game::Instance()->setState(Game::NEWROUND);
	}
}