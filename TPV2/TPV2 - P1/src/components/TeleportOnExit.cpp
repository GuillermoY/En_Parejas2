// This file is part of the course TPV2@UCM - Samir Genaim

#include "TeleportOnExit.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

TeleportOnExit::TeleportOnExit() :
		_tr(nullptr) {
}

TeleportOnExit::~TeleportOnExit() {
}

void TeleportOnExit::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void TeleportOnExit::update() {
	auto &pos = _tr->getPos();
	float w   = _tr->getWidth();
	float h   = _tr->getHeight();
	int   sw  = sdlutils().width();
	int   sh  = sdlutils().height();

	// Si sale completamente por cualquier borde, teleportar a borde aleatorio
	bool outRight  = pos.getX() > sw;
	bool outLeft   = pos.getX() + w < 0;
	bool outBottom = pos.getY() > sh;
	bool outTop    = pos.getY() + h < 0;

	if (outRight || outLeft || outBottom || outTop) {
		auto &r   = sdlutils().rand();
		int border = r.nextInt(0, 4); // 0=top, 1=bottom, 2=left, 3=right
		float nx, ny;
		switch (border) {
		case 0: // top
			nx = static_cast<float>(r.nextInt(0, sw));
			ny = -h;
			break;
		case 1: // bottom
			nx = static_cast<float>(r.nextInt(0, sw));
			ny = static_cast<float>(sh);
			break;
		case 2: // left
			nx = -w;
			ny = static_cast<float>(r.nextInt(0, sh));
			break;
		default: // right
			nx = static_cast<float>(sw);
			ny = static_cast<float>(r.nextInt(0, sh));
			break;
		}
		pos.set(nx, ny);
	}
}
