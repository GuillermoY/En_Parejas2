// This file is part of the course TPV2@UCM - Samir Genaim

#include "PaddlesSystem.h"

#include "../components/PaddleCtrl.h"
#include "../components/RectangleViewer.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

PaddlesSystem::PaddlesSystem() :
		_ballTr(nullptr) {
}

PaddlesSystem::~PaddlesSystem() {
}

void PaddlesSystem::initSystem() {
	auto ballEntity = _mngr->getHandler(ecs::hdlr::BALL);
	_ballTr = _mngr->getComponent<Transform>(ballEntity);
	assert(_ballTr != nullptr);

	// create the left paddle
	auto leftPaddle = _mngr->addEntity(ecs::grp::PADDLES);

	auto leftPaddleTr = _mngr->addComponent<Transform>(leftPaddle);
	auto leftPaddleWidth = 10.0f;
	auto leftPaddleHeight = 50.0f;
	auto leftPaddleX = 5.f;
	auto leftPaddleY = (sdlutils().height() - leftPaddleHeight) / 2.0f;
	leftPaddleTr->init(Vector2D(leftPaddleX, leftPaddleY), Vector2D(),
			leftPaddleWidth, leftPaddleHeight, 0.0f);

	_mngr->addComponent<RectangleViewer>(leftPaddle,
			build_sdlcolor(0xff0000ff));
	auto leftPaddleCtrl = _mngr->addComponent<PaddleCtrl>(leftPaddle);
//	leftPaddleCtrl->setType(PaddleCtrl::MOUSE);
	leftPaddleCtrl->setType(PaddleCtrl::AI);

	// create the right paddle
	auto rightPaddle = _mngr->addEntity(ecs::grp::PADDLES);

	auto rightPaddleTr = _mngr->addComponent<Transform>(rightPaddle);
	auto rightPaddleWidth = 10.0f;
	auto rightPaddleHeight = 50.0f;
	auto rightPaddleX = sdlutils().width() - rightPaddleWidth - 5.0f;
	auto rightPaddleY = (sdlutils().height() - rightPaddleHeight) / 2.0f;
	rightPaddleTr->init(Vector2D(rightPaddleX, rightPaddleY), Vector2D(),
			rightPaddleWidth, rightPaddleHeight, 0.0f);

	_mngr->addComponent<RectangleViewer>(rightPaddle,
			build_sdlcolor(0x00ff00ff));
	auto rightaddleCtrl = _mngr->addComponent<PaddleCtrl>(rightPaddle);
	rightaddleCtrl->setType(PaddleCtrl::MOUSE);
//	rightaddleCtrl->setType(PaddleCtrl::AI);

}

void PaddlesSystem::update() {
	for (auto e : _mngr->getEntities(ecs::grp::PADDLES)) {
		movePaddle(e);
	}
}

void PaddlesSystem::movePaddle(ecs::entity_t paddle) {
	auto paddleCtrl = _mngr->getComponent<PaddleCtrl>(paddle);
	auto paddleTr = _mngr->getComponent<Transform>(paddle);

	switch (paddleCtrl->_ctrlType) {
	case PaddleCtrl::KEYBOARD:
		moveWithKeyboard(paddleCtrl, paddleTr);
		break;
	case PaddleCtrl::MOUSE:
		moveWithMouse(paddleCtrl, paddleTr);
		break;
	case PaddleCtrl::AI:
		moveWithAI(paddleCtrl, paddleTr);
		break;
	default:
		break;
	}

	paddleTr->move();

	// check collision with upper/lower borders
	if (paddleTr->_pos.getY() < 0) {
		paddleTr->_pos.setY(0.0f);
		paddleTr->_vel.set(0.0f, 0.0f);
	} else if (paddleTr->_pos.getY() + paddleTr->_height
			> sdlutils().height()) {
		paddleTr->_pos.setY(sdlutils().height() - paddleTr->_height);
		paddleTr->_vel.set(0.0f, 0.0f);
	}

}

void PaddlesSystem::moveWithKeyboard(PaddleCtrl *paddleCtrl,
		Transform *paddleTr) {
	auto &ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(paddleCtrl->_up)) {
			paddleTr->_vel.setY(-paddleCtrl->_speed);
		} else if (ihdlr.isKeyDown(paddleCtrl->_down)) {
			paddleTr->_vel.setY(paddleCtrl->_speed);
		} else if (ihdlr.isKeyDown(paddleCtrl->_stop)) {
			paddleTr->_vel.setY(0.0f);
		}
	}
}

void PaddlesSystem::moveWithMouse(PaddleCtrl *paddleCtrl, Transform *paddleTr) {
	auto &ihdlr = ih();

	if (ihdlr.mouseMotionEvent()) {
		paddleTr->_pos.setY(ihdlr.getMousePos().second);
	}
}

void PaddlesSystem::moveWithAI(PaddleCtrl *paddleCtrl, Transform *paddleTr) {
	if (ballComingTowardsPaddle(paddleTr->_pos)) {
		float diff = (_ballTr->_pos.getY() + _ballTr->_height / 2.0f)
				- (paddleTr->_pos.getY() + paddleTr->_height / 2.0f);
		if (abs(diff) < 10) {
			paddleTr->_vel.setY(0.0f);
		} else {
			paddleTr->_vel.setY(
					(diff > 0.0f ? 1.0f : -1.0f) * 1.0f
							* _ballTr->_vel.magnitude());
		}
	} else {
		float diff = (sdlutils().height() / 2.0f)
				- (paddleTr->_pos.getY() + paddleTr->_height / 2.0f);
		if (abs(diff) < 10) {
			paddleTr->_vel.setY(0.0);
		} else {
			paddleTr->_vel.setY((diff > 0 ? 1 : -1) * 5.0f);
		}
	}
}

bool PaddlesSystem::ballComingTowardsPaddle(const Vector2D &pos) {
	return (pos.getX() < _ballTr->_pos.getX() && _ballTr->_vel.getX() < 0)
			|| (pos.getX() > _ballTr->_pos.getX() && _ballTr->_vel.getX() > 0);
}

