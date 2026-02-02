// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <array>

#include "Container.h"

class GameManager: public Container {
public:

	enum Side {
		LEFT = 0, //
		RIGHT
	};

	GameManager();
	virtual ~GameManager();

	inline void setScore(std::size_t player, unsigned int score) {
		_score[player] = score;
	}

	inline unsigned int getScore(std::size_t player) {
		return _score[player];
	}

	void onBallExit(Side side);

	void resetScore() {
		_score[0] = _score[1] = 0;
	}

private:
	std::array<unsigned int, 2> _score;
	const unsigned int _maxScore;
};

