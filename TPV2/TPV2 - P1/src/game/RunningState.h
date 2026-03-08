// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cstdint>
#include "GameState.h"

class RunningState : public GameState {
public:
	RunningState();
	virtual ~RunningState();
	void enter()  override;
	void leave()  override;
	void update() override;

private:
	void checkCollisions();
	uint64_t _lastAsteroidTime;
};
