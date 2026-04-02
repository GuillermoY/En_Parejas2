// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>

#include "../ecs/Component.h"

struct GameState: ecs::Component {

	enum Side {
		LEFT = 0, //
		RIGHT
	};

	enum State {
		NEWGAME = 0, // just before starting a new game
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	GameState() :
			_score(), //
			_state(NEWGAME), //
			_maxScore(5) {
	}
	virtual ~GameState() {
	}

	inline State getState() {
		return _state;
	}

	inline void setState(State state) {
		_state = state;
	}

	inline void setScore(std::size_t player, unsigned int score) {
		_score[player] = score;
	}

	inline unsigned int getScore(std::size_t player) {
		return _score[player];
	}


	std::array<unsigned int, 2> _score;
	State _state;
	const unsigned int _maxScore;

};

