// This file is part of the course TPV2@UCM - Samir Genaim

#include "DeAcceleration.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "Transform.h"

DeAcceleration::DeAcceleration() :
	_tr(nullptr) {
}

DeAcceleration::~DeAcceleration() {
}

void DeAcceleration::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void DeAcceleration::update() {
	auto& vel = _tr->getVel();
	vel = vel * _DEACCEL_FACTOR;
	if (vel.magnitude() < _MIN_SPEED) {
		vel.set(0.0f, 0.0f);
	}
}