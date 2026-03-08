// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class NewGameState : public GameState {
public:
	NewGameState();
	virtual ~NewGameState();
	void enter()  override;
	void leave()  override;
	void update() override;
};
