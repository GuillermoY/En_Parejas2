// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/ImmunitySystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"

#include "Newgamestate.h"
#include "Newroundstate.h"
#include "Runningstate.h"
#include "Pausestate.h"
#include "Gameoverstate.h"

Game* Game::_instance = nullptr;

Game::Game() :
	_mngr(nullptr),
	_state(nullptr),
	_newGameState(nullptr),
	_newRoundState(nullptr),
	_runningState(nullptr),
	_pauseState(nullptr),
	_gameOverState(nullptr) {
	assert(_instance == nullptr);
	_instance = this;
}

Game::~Game() {
	delete _newGameState;
	delete _newRoundState;
	delete _runningState;
	delete _pauseState;
	delete _gameOverState;
	delete _mngr;

	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

static void init() {
	if (!SDLUtils::Init("PacMan - TPV2@UCM", 800, 600,
		"resources/config/pacman.resources.json")) {
		throw std::string("Could not initialise SDLUtils");
	}

	if (!InputHandler::Init()) {
		throw std::string("Could not initialise InputHandler");
	}
}

void Game::initGame() {
	_mngr = new ecs::EntityManager();

	_mngr->addSystem<RenderSystem>();
	_mngr->addSystem<PacManSystem>();
	_mngr->addSystem<FoodSystem>();
	_mngr->addSystem<GhostSystem>();
	_mngr->addSystem<ImmunitySystem>();
	_mngr->addSystem<CollisionsSystem>();

	_newGameState = new NewGameState(_mngr);
	_newRoundState = new NewRoundState(_mngr);
	_runningState = new RunningState(_mngr);
	_pauseState = new PauseState(_mngr);
	_gameOverState = new GameOverState(_mngr);

	_state = _newGameState;
	_state->enter();
}

void Game::setState(GameState* s) {
	_state->leave();
	_state = s;
	_state->enter();
}

void Game::start() {
	bool exit = false;
	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		ihdlr.refresh();

		sdlutils().virtualTimer().regCurrTime();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
		}
		else
		{
			_state->update();

			_mngr->flushMessages();
			_mngr->refresh();

			Uint32 frameTime = sdlutils().currRealTime() - startTime;
			if (frameTime < 10)
				SDL_Delay(10 - frameTime);
		}
	}
}