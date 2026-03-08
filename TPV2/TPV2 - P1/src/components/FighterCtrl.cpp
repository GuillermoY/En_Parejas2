// This file is part of the course TPV2@UCM - Samir Genaim

#include "FighterCtrl.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

FighterCtrl::FighterCtrl() :
	_tr(nullptr) {
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void FighterCtrl::update() {
	auto& ihdlr = ih();
	auto& vel = _tr->getVel();
	auto  rot = _tr->getRot();

	// Girar izquierda / derecha
	if (ihdlr.isKeyDown(SDL_SCANCODE_LEFT)) {
		_tr->setRot(rot - _ROT_STEP);
	}
	else if (ihdlr.isKeyDown(SDL_SCANCODE_RIGHT)) {
		_tr->setRot(rot + _ROT_STEP);
	}

	// Acelerar hacia arriba (dirección donde apunta la nave)
	if (ihdlr.isKeyDown(SDL_SCANCODE_UP)) {
		Vector2D new_vel = vel + Vector2D(0.0f, -1.0f).rotate(_tr->getRot()) * _THRUST;
		if (new_vel.magnitude() > _SPEED_LIMIT) {
			new_vel = new_vel.normalize() * _SPEED_LIMIT;
		}
		vel = new_vel;
		sdlutils().soundEffects().at("thrust").play("se");
	}
}