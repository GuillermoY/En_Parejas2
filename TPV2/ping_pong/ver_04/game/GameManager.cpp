// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameManager.h"

#include <cassert>

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "Container.h"

#include "GameObject.h"

GameManager::GameManager(GameObject *ball) :
		_score(), //
		_state(NEWGAME), //
		_maxScore(5), //
		_ball(ball) {
}

GameManager::~GameManager() {
}

void GameManager::onBallExit(Side side) {

	assert(_state == RUNNING); // this should be called only when game is running

	if (side == LEFT) {
		_score[1]++;
	} else {
		_score[0]++;
	}
	resetBall();
	if (_score[0] < _maxScore && _score[1] < _maxScore)
		_state = PAUSED;
	else
		_state = GAMEOVER;
}

void GameManager::resetBall() {
	_ball->getPos().set(sdlutils().width() / 2 - 5,
			sdlutils().height() / 2 - 5);
	_ball->getVel().set(0, 0);
}

void GameManager::moveBall() {
	auto &r = sdlutils().rand();

	int dx = 1 - 2 * r.nextInt(0, 2); // 1 or -1
	int dy = 1 - 2 * r.nextInt(0, 2); // 1 or -1

	Vector2D v(dx * r.nextInt(2, 7), dy * r.nextInt(2, 7));
	_ball->getVel().set(v.normalize() * 2);
}
