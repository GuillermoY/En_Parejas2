// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_rect.h>

#include "GameState.h"

class Texture;

class NewGameState: public GameState {
public:
	NewGameState();
	virtual ~NewGameState();
	void leave() override;
	void update() override;
	void enter() override;
private:
	const Texture &_msg;
};

