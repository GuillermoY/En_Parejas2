// This file is part of the course TPV2@UCM - Samir Genaim

#include "Runningstate.h"
#include "Game.h"
#include "../systems/PacManSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/ImmunitySystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/RenderSystem.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

void RunningState::update() {
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {
		Game::instance()->setState(Game::instance()->getPauseState());
		return;
	}

	_mngr->getSystem<PacManSystem>()->update();
	_mngr->getSystem<FoodSystem>()->update();
	_mngr->getSystem<GhostSystem>()->update();
	_mngr->getSystem<ImmunitySystem>()->update();
	_mngr->getSystem<CollisionsSystem>()->update();

	_mngr->getSystem<RenderSystem>()->update();
}