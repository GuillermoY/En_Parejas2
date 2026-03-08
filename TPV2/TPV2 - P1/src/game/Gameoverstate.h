// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <string>
#include "GameState.h"

class GameOverState : public GameState {
public:
	GameOverState();
	virtual ~GameOverState();
	void enter()  override;
	void leave()  override;
	void update() override;

private:
	std::string _msgKey; // clave del mensaje a mostrar ("gameover_win" o "gameover_lose")
};
