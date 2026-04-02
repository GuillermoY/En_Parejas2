// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"
#include "../ecs/EntityManager.h"

class NewGameState : public GameState {
public:
	NewGameState(ecs::EntityManager* mngr) : _mngr(mngr) {}
	void enter()  override;
	void update() override;
private:
	ecs::EntityManager* _mngr;
};