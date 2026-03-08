// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class NewRoundState : public GameState {
public:
	NewRoundState();
	virtual ~NewRoundState();
	void enter()  override;
	void leave()  override;
	void update() override;
};
