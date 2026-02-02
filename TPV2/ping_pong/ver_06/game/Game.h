// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

#include "../utils/Singleton.h"
#include "GameState.h"

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Container;
class GameObject;
class GameManager;
class PingPongFactory;

class Game: public Singleton<Game> {
	friend class Singleton<Game> ;
public:

	enum State {
		NEWGAME_STATE = 0, // just before starting a new game
		PAUSED_STATE, // between rounds
		RUNNING_STATE, // playing
		GAMEOVER_STATE // game over
	};

	Game();
	virtual ~Game();
	void initGame(PingPongFactory &facotry);
	void start();

	inline void setState(State s) {
		_state->leave();
		switch (s) {
		case NEWGAME_STATE:
			_state = _newgame_state;
			break;
		case PAUSED_STATE:
			_state = _paused_state;
			break;
		case RUNNING_STATE:
			_state = _running_state;
			break;
		case GAMEOVER_STATE:
			_state = _gameover_state;
			break;
		}
		_state->enter();
	}

	auto& getObjs() {
		return _objs;
	}

	GameObject* getBall() {
		return _ball;
	}

	GameManager* getGm() {
		return _gm;
	}

	GameObject* getLeftPaddle() {
		return _leftPaddle;
	}

	GameObject* getRightPaddle() {
		return _rightPaddle;
	}

private:
	bool init();
	void checkCollisions();

	GameManager *_gm;
	GameObject *_leftPaddle;
	GameObject *_rightPaddle;
	GameObject *_ball;
	std::vector<GameObject*> _objs;

	GameState *_state;
	GameState *_newgame_state;
	GameState *_paused_state;
	GameState *_running_state;
	GameState *_gameover_state;
};

