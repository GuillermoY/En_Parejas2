// This file is part of the course TPV2@UCM - Samir Genaim

#include "StarMotion.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "GameCtrl.h"
#include "Transform.h"


StarMotion::StarMotion() :
		_tr(), _lastUpdate() {
}

StarMotion::~StarMotion() {
}

void StarMotion::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);

	_lastUpdate = sdlutils().currTime();
}

void StarMotion::update() {

	// update every 50ms
	if (_lastUpdate + 50 > sdlutils().currTime())
		return;

	_lastUpdate = sdlutils().currTime();

	_tr->setWidth(_tr->getWidth() * 0.95f);
	_tr->setHeight(_tr->getHeight() * 0.95f);

	if (_tr->getWidth() < 5 || _tr->getHeight() < 5) {
		_ent->setAlive(false);
		auto mngr = _ent->getMngr();
		mngr->getHandler(ecs::hdlr::GAMEINFO)->getComponent<GameCtrl>()->onStarDeath();
	} else {
		_tr->setRot(_tr->getRot() + 5.0f);
	}
}
