// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class Texture;

class GameOverState: public GameState {
public:
	GameOverState();
	virtual ~GameOverState();
	void leave() override;
	void update() override;
	void enter() override;
private:
	const Texture &_msg;
};

