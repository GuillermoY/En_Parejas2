// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"
#include "../ecs/EntityManager.h"

class GameOverState : public GameState {
public:
	GameOverState(ecs::EntityManager* mngr) : _mngr(mngr), _won(false) {}
	void setWon(bool won) { _won = won; }
	void update() override;
private:
	ecs::EntityManager* _mngr;
	bool _won;
};