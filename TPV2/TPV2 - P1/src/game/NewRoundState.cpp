// This file is part of the course TPV2@UCM - Samir Genaim

#include "NewRoundState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Health.h"
#include "AsteroidsUtils.h"
#include "FighterUtils.h"
#include "Game.h"

NewRoundState::NewRoundState() {
}

NewRoundState::~NewRoundState() {
}

void NewRoundState::enter() {
}

void NewRoundState::leave() {
}

void NewRoundState::update() {
	auto &t = sdlutils().msgs().at("new_round");
	t.render(
		(sdlutils().width()  - t.width())  / 2.0f,
		(sdlutils().height() - t.height()) / 2.0f
	);

	auto *fighter = Game::Instance()->getMngr()->getHandler(ecs::hdlr::FIGHTER);
	fighter->getComponent<Health>()->render();

	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN)) {
		auto *g = Game::Instance();
		g->getFighterUtils()->reset_fighter();
		g->getAsteroidsUtils()->remove_all_asteroids();
		g->getAsteroidsUtils()->create_asteroids(10);
		g->setState(Game::RUNNING);
	}
}
