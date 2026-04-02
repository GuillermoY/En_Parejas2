// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../components/GameState.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "BallSystem.h"

GameCtrlSystem::GameCtrlSystem() :
		_score(), //
		_state(NEWGAME), //
		_maxScore(5) {
}

GameCtrlSystem::~GameCtrlSystem() {
}

void GameCtrlSystem::update() {
	if (_state != RUNNING) {

		auto &inhdlr = ih();

		if (inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
			switch (_state) {
			case NEWGAME:
				_mngr->getSystem<BallSystem>()->moveBall();
				_state = RUNNING;
				break;
			case PAUSED:
				_mngr->getSystem<BallSystem>()->moveBall();
				_state = RUNNING;
				break;
			case GAMEOVER:
				_state = NEWGAME;
				_score[0] = 0;
				_score[1] = 0;
				break;
			default:
				break;
			}
		}
	}
}

void GameCtrlSystem::onBallExit(Side side) {

	assert(_state == RUNNING); // this should be called only when game is running

	if (side == LEFT) {
		_score[1]++;
	} else {
		_score[0]++;
	}

	_mngr->getSystem<BallSystem>()->resetBall();

	if (_score[0] < _maxScore && _score[1] < _maxScore)
		_state = PAUSED;
	else
		_state = GAMEOVER;
}

