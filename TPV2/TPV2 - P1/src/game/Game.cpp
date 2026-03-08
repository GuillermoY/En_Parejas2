// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include <iostream>
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "AsteroidsUtils.h"
#include "FighterUtils.h"
#include "GameOverState.h"
#include "NewGameState.h"
#include "NewRoundState.h"
#include "PauseState.h"
#include "RunningState.h"

Game::Game() :
		_mngr(nullptr),
		_fighterUtils(nullptr),
		_asteroidsUtils(nullptr),
		_state(nullptr),
		_runningState(nullptr),
		_pausedState(nullptr),
		_newGameState(nullptr),
		_newRoundState(nullptr),
		_gameOverState(nullptr) {
}

Game::~Game() {
	delete _mngr;
	delete _fighterUtils;
	delete _asteroidsUtils;
	delete _runningState;
	delete _pausedState;
	delete _newGameState;
	delete _newRoundState;
	delete _gameOverState;

	if (InputHandler::HasInstance())
		InputHandler::Release();
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {
	return true;
}

bool Game::initSDL() {
	if (!SDLUtils::Init("Asteroids", 800, 600,
			"resources/config/asteroid.resources.json")) {
		std::cerr << "Error initializing SDLUtils" << std::endl;
		return false;
	}
	if (!InputHandler::Init()) {
		std::cerr << "Error initializing InputHandler" << std::endl;
		return false;
	}
	return true;
}

void Game::initGame() {
	_mngr = new ecs::EntityManager();

	_fighterUtils   = new FighterUtils();
	_asteroidsUtils = new AsteroidsUtils();

	_fighterUtils->create_fighter();

	_runningState  = new RunningState();
	_pausedState   = new PauseState();
	_newGameState  = new NewGameState();
	_newRoundState = new NewRoundState();
	_gameOverState = new GameOverState();

	_state = _newGameState;
	_state->enter();
}

void Game::setState(State s) {
	_state->leave();
	switch (s) {
	case RUNNING:  _state = _runningState;  break;
	case PAUSED:   _state = _pausedState;   break;
	case NEWGAME:  _state = _newGameState;  break;
	case NEWROUND: _state = _newRoundState; break;
	case GAMEOVER: _state = _gameOverState; break;
	default: break;
	}
	_state->enter();
}

void Game::start() {
	bool exit = false;
	auto &ihdlr = ih();
	auto &vt    = sdlutils().virtualTimer();
	vt.resetTime();

	while (!exit) {
		Uint32 startTime = static_cast<Uint32>(sdlutils().currRealTime());

		vt.regCurrTime();

		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		sdlutils().clearRenderer();
		_state->update();
		sdlutils().presentRenderer();

		Uint32 frameTime = static_cast<Uint32>(sdlutils().currRealTime()) - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}
