// This file is part of the course TPV2@UCM - Samir Genaim

#include "TowardDestination.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

TowardDestination::TowardDestination() :
		_tr(nullptr), _dest() {
}

TowardDestination::~TowardDestination() {
}

void TowardDestination::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
	pickNewDestination();
}

void TowardDestination::pickNewDestination() {
	auto &r = sdlutils().rand();
	float cx = sdlutils().width()  / 2.0f;
	float cy = sdlutils().height() / 2.0f;
	float rx = static_cast<float>(r.nextInt(-100, 101));
	float ry = static_cast<float>(r.nextInt(-100, 101));
	_dest = Vector2D(cx + rx, cy + ry);
}

void TowardDestination::update() {
	auto &v = _tr->getVel();
	auto &p = _tr->getPos();

	// Si llegamos al destino, elegimos uno nuevo
	if ((p - _dest).magnitude() < _ARRIVAL_DIST) {
		pickNewDestination();
	}

	float angle = v.angle(_dest - p);
	v = v.rotate(angle);
}
