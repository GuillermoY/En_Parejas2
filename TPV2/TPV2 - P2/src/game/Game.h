// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/ecs.h"
#include "../ecs/EntityManager.h"
#include "GameState.h"
#include "messages_defs.h"

class NewGameState;
class NewRoundState;
class RunningState;
class PauseState;
class GameOverState;

class Game {
public:
	Game();
	virtual ~Game();

	void init();
	void start();

	void setState(GameState* s);

	NewGameState* getNewGameState() { return _newGameState; }
	NewRoundState* getNewRoundState() { return _newRoundState; }
	RunningState* getRunningState() { return _runningState; }
	PauseState* getPauseState() { return _pauseState; }
	GameOverState* getGameOverState() { return _gameOverState; }

	ecs::EntityManager* getMngr() { return _mngr; }

	static Game* instance() { return _instance; }

private:
	static Game* _instance;

	ecs::EntityManager* _mngr;
	GameState* _state;

	NewGameState* _newGameState;
	NewRoundState* _newRoundState;
	RunningState* _runningState;
	PauseState* _pauseState;
	GameOverState* _gameOverState;
};