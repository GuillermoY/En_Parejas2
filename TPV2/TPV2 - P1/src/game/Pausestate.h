// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class PauseState : public GameState {
public:
	PauseState();
	virtual ~PauseState();
	void enter()  override;
	void leave()  override;
	void update() override;
};
