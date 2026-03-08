// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../utils/Singleton.h"

namespace ecs {
class EntityManager;
}

class GameState;
class FighterUtils;
class AsteroidsUtils;

#include "../components/Health.h"
#include "../ecs/EntityManager.h"
#include "ecs_defs.h"

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:

	enum State {
		RUNNING,
		PAUSED,
		NEWGAME,
		NEWROUND,
		GAMEOVER
	};

	bool initSDL();
	void initGame();
	void start();

	inline ecs::EntityManager* getMngr() { return _mngr; }
	inline FighterUtils* getFighterUtils() { return _fighterUtils; }
	inline AsteroidsUtils* getAsteroidsUtils() { return _asteroidsUtils; }

	void setState(State s);

private:
	Game();
	virtual ~Game();
	bool init();

	ecs::EntityManager *_mngr;
	FighterUtils       *_fighterUtils;
	AsteroidsUtils     *_asteroidsUtils;

	GameState *_state;
	GameState *_runningState;
	GameState *_pausedState;
	GameState *_newGameState;
	GameState *_newRoundState;
	GameState *_gameOverState;
};
