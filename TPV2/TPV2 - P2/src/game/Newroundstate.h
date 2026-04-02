// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"
#include "../ecs/EntityManager.h"

class NewRoundState : public GameState {
public:
	NewRoundState(ecs::EntityManager* mngr) : _mngr(mngr) {}
	void update() override;
private:
	ecs::EntityManager* _mngr;
};