// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"
#include "../ecs/EntityManager.h"

class RunningState : public GameState {
public:
	RunningState(ecs::EntityManager* mngr) : _mngr(mngr) {}
	void update() override;
private:
	ecs::EntityManager* _mngr;
};