// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameManager.h"

#include <cassert>
#include <string>

#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

GameManager::GameManager(GameObject *ball) :
		_score(), //
		_state(NEWGAME), //
		_maxScore(5), //
		_ball(ball), //
		_scoreMsg() {
	updateScoreMsg();
}

GameManager::~GameManager() {
}

void GameManager::handleInput(const SDL_Event &event) {
	if (_state != RUNNING) {
		if (event.type == SDL_EVENT_KEY_DOWN
				&& event.key.scancode == SDL_SCANCODE_SPACE) {
			switch (_state) {
			case NEWGAME:
				moveBall();
				_state = RUNNING;
				break;
			case PAUSED:
				moveBall();
				_state = RUNNING;
				break;
			case GAMEOVER:
				_state = NEWGAME;
				_score[0] = _score[1] = 0;
				updateScoreMsg();
				break;
			default:
				break;
			}
		}
	}
}

void GameManager::update() {
}

void GameManager::render() {

	// message when game is not running
	if (_state != RUNNING) {

		// game over message
		if (_state == GAMEOVER) {
			auto &t = sdlutils().msgs().at("gameover");
			t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
		}

		// new game message
		if (_state == NEWGAME) {
			auto &t = sdlutils().msgs().at("start");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		} else {
			auto &t = sdlutils().msgs().at("continue");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		}
	}

	// score
	_scoreMsg.render((sdlutils().width() - _scoreMsg.width()) / 2, 10);

}

void GameManager::onBallExit(Side side) {

	assert(_state == RUNNING); // this should be called only when game is runnnig

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

	updateScoreMsg();
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

void GameManager::updateScoreMsg() {
	_scoreMsg = Texture(
			sdlutils().renderer(), //
			std::to_string(_score[0]) + " - " + std::to_string(_score[1]),
			sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
}
