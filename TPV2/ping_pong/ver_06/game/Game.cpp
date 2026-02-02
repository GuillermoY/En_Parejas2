// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "Container.h"
#include "GameManager.h"
#include "AIPaddle.h"
#include "BounceOnBorder.h"
#include "EmptyRectangleRenderer.h"
#include "GameOverState.h"
#include "ImageRenderer.h"
#include "MovePaddleWithKeyBoard.h"
#include "MovePaddleWithMouse.h"
#include "NewGameState.h"
#include "PausedState.h"
#include "PingPongFactory.h"
#include "RectangleRenderer.h"
#include "RunningState.h"
#include "ScoreRenderer.h"
#include "SimpleMove.h"
#include "StopOnBorder.h"

Game::Game() :
		_gm(nullptr), //
		_leftPaddle(nullptr), //
		_rightPaddle(nullptr), //
		_ball(nullptr), //
		_state(nullptr), //
		_newgame_state(nullptr), //
		_paused_state(nullptr), //
		_running_state(nullptr), //
		_gameover_state(nullptr) //
{
}

Game::~Game() {

	delete _gameover_state;
	delete _running_state;
	delete _paused_state;
	delete _newgame_state;

	// delete all game objects
	for (GameObject *o : _objs) {
		delete o;
	}

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

bool Game::init() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("Ping Pong", 800, 600,
			"resources/config/pingpong.resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return false;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return false;

	}

	return true;
}

void Game::initGame(PingPongFactory &facotry) {

	_ball = facotry.createBall();
	_leftPaddle = facotry.createLeftPaddle(_ball);
	_rightPaddle = facotry.createRightPaddle(_ball);
	_gm = facotry.createGameManager(_ball, _leftPaddle, _rightPaddle);

	// add them all to the list of game objects
	_objs.push_back(_ball);
	_objs.push_back(_leftPaddle);
	_objs.push_back(_rightPaddle);
	_objs.push_back(_gm);

	_newgame_state = new NewGameState();
	_paused_state = new PausedState();
	_running_state = new RunningState();
	_gameover_state = new GameOverState();

	_state = _newgame_state;

	_state->enter();

}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {

		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = sdlutils().regCurrTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		_state->update();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}

