// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "GameCtrlSystem.h"

CollisionsSystem::CollisionsSystem() :
		_ballTr(nullptr) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	_ballTr = _mngr->getComponent<Transform>(
			_mngr->getHandler(ecs::hdlr::BALL));
	assert(_ballTr != nullptr);
}

void CollisionsSystem::update() {

	auto gameCtrlSystem = _mngr->getSystem<GameCtrlSystem>();

	if (gameCtrlSystem->getState() != GameCtrlSystem::RUNNING)
		return;

	bool ballCollidesWithPaddle = false;

	for (auto e : _mngr->getEntities(ecs::grp::PADDLES)) {
		auto paddleTr_ = _mngr->getComponent<Transform>(e);
		ballCollidesWithPaddle = Collisions::collides(paddleTr_->_pos,
				paddleTr_->_width, paddleTr_->_height, _ballTr->_pos,
				_ballTr->_width, _ballTr->_height);

		if (ballCollidesWithPaddle)
			break;
	}

	if (ballCollidesWithPaddle) {

		// change the direction of the ball, and increment the speed
		auto &vel = _ballTr->_vel; // the use of & is important, so the changes goes directly to the ball
		vel.setX(-vel.getX());
		vel = vel * 1.2f;

		// play some sound
		sdlutils().soundEffects().at("paddle_hit").play("se");
	} else if (_ballTr->_pos.getX() < 0)
		gameCtrlSystem->onBallExit(GameCtrlSystem::LEFT);
	else if (_ballTr->_pos.getX() + _ballTr->_width > sdlutils().width())
		gameCtrlSystem->onBallExit(GameCtrlSystem::RIGHT);

}
