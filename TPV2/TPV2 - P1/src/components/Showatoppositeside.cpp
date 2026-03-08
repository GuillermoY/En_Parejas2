// This file is part of the course TPV2@UCM - Samir Genaim

#include "ShowAtOppositeSide.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

ShowAtOppositeSide::ShowAtOppositeSide() :
	_tr(nullptr) {
}

ShowAtOppositeSide::~ShowAtOppositeSide() {
}

void ShowAtOppositeSide::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void ShowAtOppositeSide::update() {
	auto& pos = _tr->getPos();
	float w = _tr->getWidth();
	float h = _tr->getHeight();
	int   sw = sdlutils().width();
	int   sh = sdlutils().height();

	// Sale completamente por la derecha → aparece por la izquierda
	if (pos.getX() > sw) {
		pos.setX(-w);
	}
	// Sale completamente por la izquierda → aparece por la derecha
	else if (pos.getX() + w < 0) {
		pos.setX(static_cast<float>(sw));
	}

	// Sale completamente por abajo → aparece por arriba
	if (pos.getY() > sh) {
		pos.setY(-h);
	}
	// Sale completamente por arriba → aparece por abajo
	else if (pos.getY() + h < 0) {
		pos.setY(static_cast<float>(sh));
	}
}