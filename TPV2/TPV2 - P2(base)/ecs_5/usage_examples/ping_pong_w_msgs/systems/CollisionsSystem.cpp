// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/messages.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "GameCtrlSystem.h"

CollisionsSystem::CollisionsSystem() :
		_ballTr(nullptr), _active(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	_ballTr = _mngr->getComponent<Transform>(
			_mngr->getHandler(ecs::hdlr::BALL));
	assert(_ballTr != nullptr);
}

void CollisionsSystem::update() {
	if (!_active)
		return;

	bool hit = false;
	ecs::entity_t paddleHit;
	for (auto e : _mngr->getEntities(ecs::grp::PADDLES)) {
		auto paddleTr_ = _mngr->getComponent<Transform>(e);
		if (Collisions::collides(paddleTr_->_pos, paddleTr_->_width,
				paddleTr_->_height, _ballTr->_pos, _ballTr->_width,
				_ballTr->_height)) {
			hit = true;
			paddleHit = e;
			break;
		}
	}

	if (hit) {
		Message m;
		m.id = _m_BALL_HIT_PADDLE;
		m.ball_hit_paddle.e = paddleHit;
		_mngr->send(m);

	} else if (_ballTr->_pos.getX() < 0) {
		Message m;
		m.id = _m_BALL_EXIT;
		m.ball_exit.side = GameCtrlSystem::LEFT;
		_mngr->send(m);
	} else if (_ballTr->_pos.getX() + _ballTr->_width > sdlutils().width()) {
		Message m;
		m.id = _m_BALL_EXIT;
		m.ball_exit.side = GameCtrlSystem::RIGHT;
		_mngr->send(m);
	}
}

void CollisionsSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_ROUND_START:
		_active = true;
		break;
	case _m_ROUND_OVER:
		_active = false;
		break;
	default:
		break;
	}
}
