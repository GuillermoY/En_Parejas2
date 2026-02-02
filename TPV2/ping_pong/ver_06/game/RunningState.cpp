// This file is part of the course TPV2@UCM - Samir Genaim

#include "RunningState.h"

#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "Game.h"
#include "Container.h"
#include "GameManager.h"

RunningState::RunningState() {
	_gm = Game::Instance()->getGm();
	_leftPaddle = Game::Instance()->getLeftPaddle();
	_rightPaddle = Game::Instance()->getRightPaddle();
	_ball = Game::Instance()->getBall();
}

RunningState::~RunningState() {
	// TODO Auto-generated destructor stub
}

void RunningState::leave() {
}

void RunningState::update() {
	sdlutils().clearRenderer();

	for (auto &o : Game::Instance()->getObjs()) {
		o->handleInput();
	}

	// update
	for (auto &o : Game::Instance()->getObjs()) {
		o->update();
	}

	checkCollisions();

	sdlutils().clearRenderer();

	// render
	for (auto &o : Game::Instance()->getObjs()) {
		o->render();
	}

	sdlutils().presentRenderer();
}

void RunningState::enter() {
	auto &r = sdlutils().rand();

	int dx = 1 - 2 * r.nextInt(0, 2); // 1 or -1
	int dy = 1 - 2 * r.nextInt(0, 2); // 1 or -1

	Vector2D v(dx * r.nextInt(2, 7), dy * r.nextInt(2, 7));
	_ball->getVel().set(v.normalize() * 2);
}

void RunningState::checkCollisions() {

	// check if ball hits paddles
	if (Collisions::collides(_leftPaddle->getPos(), _leftPaddle->getWidth(),
			_leftPaddle->getHeight(), _ball->getPos(), _ball->getWidth(),
			_ball->getHeight())
			|| Collisions::collides(_rightPaddle->getPos(),
					_rightPaddle->getWidth(), _rightPaddle->getHeight(),
					_ball->getPos(), _ball->getWidth(), _ball->getHeight())) {

		// change the direction of the ball, and increment the speed
		auto &vel = _ball->getVel(); // the use of & is important, so the changes goes directly to the ball
		vel.setX(-vel.getX());
		vel = vel * 1.2f;

		// play some sound
		sdlutils().soundEffects().at("paddle_hit").play();
	} else if (_ball->getPos().getX() < 0)
		_gm->onBallExit(GameManager::LEFT);
	else if (_ball->getPos().getX() + _ball->getWidth() > sdlutils().width())
		_gm->onBallExit(GameManager::RIGHT);
}

