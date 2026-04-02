// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cassert>
#include "../ecs/Component.h"

struct Health : public ecs::Component {

	static constexpr int INITIAL_LIVES = 3;

	Health() : _lives(INITIAL_LIVES) {}

	virtual ~Health() {}

	void reset() {
		_lives = INITIAL_LIVES;
	}

	int loseLife() {
		assert(_lives > 0);
		return --_lives;
	}

	int _lives;
};