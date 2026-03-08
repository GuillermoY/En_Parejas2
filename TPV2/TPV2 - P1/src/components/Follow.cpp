// This file is part of the course TPV2@UCM - Samir Genaim

#include "Follow.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../game/Game.h"
#include "Transform.h"

Follow::Follow() :
		_tr(nullptr), _fighterTr(nullptr) {
}

Follow::~Follow() {
}

void Follow::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);

	auto *fighter = Game::Instance()->getMngr()->getHandler(ecs::hdlr::FIGHTER);
	_fighterTr = fighter->getComponent<Transform>();
	assert(_fighterTr != nullptr);
}

void Follow::update() {
	auto &v = _tr->getVel();
	auto &p = _tr->getPos();
	auto &q = _fighterTr->getPos();

	// Rotar velocidad hacia el caza
	float angle = v.angle(q - p);
	v = v.rotate(angle > 0 ? 1.0f : -1.0f);
}
