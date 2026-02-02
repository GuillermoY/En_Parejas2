// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameManager.h"

#include <cassert>

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "Container.h"
#include "Game.h"

#include "GameObject.h"

GameManager::GameManager() :
		_score(), //
		_maxScore(1) {
}

GameManager::~GameManager() {
}

void GameManager::onBallExit(Side side) {

	if (side == LEFT) {
		_score[1]++;
	} else {
		_score[0]++;
	}

	if (_score[0] < _maxScore && _score[1] < _maxScore)
		Game::Instance()->setState(Game::PAUSED_STATE);
	else
		Game::Instance()->setState(Game::GAMEOVER_STATE);
}
