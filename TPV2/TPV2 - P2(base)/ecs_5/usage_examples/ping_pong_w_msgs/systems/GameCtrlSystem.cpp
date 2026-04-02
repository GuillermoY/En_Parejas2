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
			case PAUSED:
				startRound();
				break;
			case GAMEOVER:
				startGame();
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


	if (_score[0] < _maxScore && _score[1] < _maxScore) {
		killPacMan();
	} else {
		gameOver();
	}
}

void GameCtrlSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_BALL_EXIT:
		onBallExit(static_cast<Side>(m.ball_exit.side));
		break;
	default:
		break;
	}
}

void GameCtrlSystem::startRound() {
	Message m;
	_state = RUNNING;
	m.id = _m_ROUND_START;
	_mngr->send(m);
}

void GameCtrlSystem::startGame() {
	Message m;
	_state = NEWGAME;
	_score[0] = 0;
	_score[1] = 0;
	m.id = _m_NEW_GAME;
	_mngr->send(m);
}

void GameCtrlSystem::killPacMan() {
	Message m;
	_state = PAUSED;
	m.id = _m_ROUND_OVER;
	_mngr->send(m);
}

void GameCtrlSystem::gameOver() {
	Message m;
	_state = GAMEOVER;
	m.id = _m_ROUND_OVER;
	_mngr->send(m);
	m.id = _m_GAME_OVER;
	_mngr->send(m);
}
