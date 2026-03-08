// This file is part of the course TPV2@UCM - Samir Genaim

#include "Gun.h"

#include <cassert>
#include <cmath>
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/macros.h"
#include "Transform.h"

Gun::Gun() :
	_tr(nullptr),
	_bullets(),
	_lastUsed(0),
	_lastShotTime(0u) {
}

Gun::~Gun() {
}

void Gun::initComponent() {
	_tr = _ent->getComponent<Transform>();
	assert(_tr != nullptr);
}

void Gun::reset() {
	for (auto& b : _bullets) {
		b.used = false;
	}
	_lastUsed = 0;
	_lastShotTime = 0;
}

void Gun::update() {
	// Mover balas activas
	for (auto& b : _bullets) {
		if (b.used) {
			b.pos = b.pos + b.vel;

			// Si sale de la ventana, marcarla como no usada
			auto w = sdlutils().width();
			auto h = sdlutils().height();
			if (b.pos.getX() < 0 || b.pos.getX() > w ||
				b.pos.getY() < 0 || b.pos.getY() > h) {
				b.used = false;
			}
		}
	}

	// Disparar si el usuario pulsa S y ha pasado suficiente tiempo
	auto& ihdlr = ih();
	if (ihdlr.isKeyDown(SDL_SCANCODE_S)) {
		uint64_t now = sdlutils().virtualTimer().currTime();
		if (now - _lastShotTime >= static_cast<Uint64>(_SHOT_INTERVAL)) {
			auto& p = _tr->getPos();
			auto& v = _tr->getVel();
			float r = _tr->getRot();
			float w = _tr->getWidth();
			float h = _tr->getHeight();
			shoot(p, v, static_cast<int>(w), static_cast<int>(h), r);
			_lastShotTime = now;
			sdlutils().soundEffects().at("fire").play("se");
		}
	}
}

void Gun::render() {
	auto& fireTex = sdlutils().images().at("fire");
	for (auto& b : _bullets) {
		if (b.used) {
			SDL_FRect dest = {
				b.pos.getX(),
				b.pos.getY(),
				static_cast<float>(b.width),
				static_cast<float>(b.height)
			};
			fireTex.render(dest, b.rot);
		}
	}
}

void Gun::shoot(Vector2D p, Vector2D v, int w, int h, float r) {
	int bw = 5;
	int bh = 20;

	Vector2D c = p + Vector2D(w / 2.0f, h / 2.0f);
	Vector2D bp = c - Vector2D(bw / 2.0f, h / 2.0f + 5.0f + bh).rotate(r)
		- Vector2D(bw / 2.0f, bh / 2.0f);
	Vector2D bv = Vector2D(0.0f, -1.0f).rotate(r) * (v.magnitude() + 5.0f);
	float    br = Vector2D(0.0f, -1.0f).angle(bv);

	// Búsqueda circular a partir de _lastUsed
	for (uint8_t i = 0; i < _max_bullets; i++) {
		uint8_t idx = (_lastUsed + 1 + i) % _max_bullets;
		if (!_bullets[idx].used) {
			_bullets[idx].used = true;
			_bullets[idx].pos = bp;
			_bullets[idx].vel = bv;
			_bullets[idx].width = bw;
			_bullets[idx].height = bh;
			_bullets[idx].rot = br;
			_lastUsed = idx;
			return;
		}
	}
	// Si no hay bala libre, no se dispara
}