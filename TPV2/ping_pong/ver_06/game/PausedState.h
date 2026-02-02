// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class Texture;

class PausedState: public GameState {
public:
	PausedState();
	virtual ~PausedState();
	void leave() override;
	void update() override;
	void enter() override;
private:
	const Texture &_msg;
};

