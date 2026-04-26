// This file is part of the course TPV2@UCM - Samir Genaim

#include "ImmunitySystem.h"

#include "../components/Immunity.h"
#include "../ecs/EntityManager.h"
#include "../game/messages_defs.h"
#include "../sdlutils/SDLUtils.h"

ImmunitySystem::ImmunitySystem() {}
ImmunitySystem::~ImmunitySystem() {}
void ImmunitySystem::initSystem() {}

void ImmunitySystem::update() {
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto immunity = _mngr->getComponent<Immunity>(pm);

	if (!immunity->_active) return;

	if (immunity->isExpired(sdlutils().virtualTimer().currTime())) {
		immunity->deactivate();

		Message m;
		m.id = _m_IMMUNITY_END;
		_mngr->send(m);
	}
}

void ImmunitySystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_PACMAN_FOOD_COLLISION:
		if (m.food_collision_data.miraculous) {
			auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
			auto immunity = _mngr->getComponent<Immunity>(pm);
			if (!immunity->_active) {
				immunity->activate(sdlutils().virtualTimer().currTime());
				Message im;
				im.id = _m_IMMUNITY_START;
				_mngr->send(im);
			}
		}
		break;
	case _m_ROUND_START:
	case _m_NEW_GAME:
	{
		auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
		_mngr->getComponent<Immunity>(pm)->deactivate();
	}
	break;
	default:
		break;
	}
}