// This file is part of the course TPV2@UCM - Samir Genaim

#include "RunningState.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Gun.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Collisions.h"
#include "AsteroidsUtils.h"
#include "Game.h"

// Factor de reducción de la caja de colisión (0.6 = 60% del tamaño real)
static constexpr float _COLLISION_FACTOR = 0.6f;

// Devuelve pos y size ajustados al factor de colisión (centrado)
static void shrinkBox(const Vector2D& pos, float w, float h,
	Vector2D& outPos, float& outW, float& outH) {
	outW = w * _COLLISION_FACTOR;
	outH = h * _COLLISION_FACTOR;
	outPos = Vector2D(
		pos.getX() + (w - outW) / 2.0f,
		pos.getY() + (h - outH) / 2.0f
	);
}

RunningState::RunningState() :
	_lastAsteroidTime(0) {
}

RunningState::~RunningState() {
}

void RunningState::enter() {
	_lastAsteroidTime = sdlutils().virtualTimer().currTime();
}

void RunningState::leave() {
}

void RunningState::update() {
	auto* g = Game::Instance();
	auto* mngr = g->getMngr();
	auto& vt = sdlutils().virtualTimer();

	// Pausa
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {
		g->setState(Game::PAUSED);
		return;
	}

	// Sin asteroides -> victoria
	if (mngr->getEntities(ecs::grp::ASTEROIDS).empty()) {
		g->setState(Game::GAMEOVER);
		return;
	}

	// Actualizar caza
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	fighter->update();

	// Actualizar asteroides
	auto& asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);
	int n = static_cast<int>(asteroids.size());
	for (int i = 0; i < n; i++) {
		if (asteroids[i]->isAlive()) {
			asteroids[i]->update();
		}
	}

	// Colisiones
	checkCollisions();

	// Limpiar entidades muertas
	mngr->refresh();

	// Añadir asteroide cada 5 segundos
	uint64_t now = vt.currTime();
	if (now - _lastAsteroidTime >= 5000) {
		g->getAsteroidsUtils()->create_asteroids(1);
		_lastAsteroidTime = now;
	}

	// Dibujar asteroides primero
	for (auto* a : mngr->getEntities(ecs::grp::ASTEROIDS)) {
		if (a->isAlive()) a->render();
	}

	// Dibujar caza y vidas encima de todo
	fighter->render();
}

void RunningState::checkCollisions() {
	auto* g = Game::Instance();
	auto* mngr = g->getMngr();
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	auto* fTr = fighter->getComponent<Transform>();
	auto* gun = fighter->getComponent<Gun>();
	auto* health = fighter->getComponent<Health>();

	// Caja reducida del caza
	Vector2D fPos; float fW, fH;
	shrinkBox(fTr->getPos(), fTr->getWidth(), fTr->getHeight(), fPos, fW, fH);

	auto& asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);
	int n = static_cast<int>(asteroids.size());

	for (int i = 0; i < n; i++) {
		auto* a = asteroids[i];
		if (!a->isAlive()) continue;

		auto* aTr = a->getComponent<Transform>();

		// Caja reducida del asteroide
		Vector2D aPos; float aW, aH;
		shrinkBox(aTr->getPos(), aTr->getWidth(), aTr->getHeight(), aPos, aW, aH);

		// Colisión asteroide - caza
		if (Collisions::collidesWithRotation(
			fPos, fW, fH, fTr->getRot(),
			aPos, aW, aH, aTr->getRot())) {

			sdlutils().soundEffects().at("explosion").play("se");
			int livesLeft = health->loseLife();
			if (livesLeft > 0) {
				g->setState(Game::NEWROUND);
			}
			else {
				g->setState(Game::GAMEOVER);
			}
			return;
		}

		// Colisión asteroide - balas
		for (auto& b : *gun) {
			if (!b.used) continue;

			if (Collisions::collidesWithRotation(
				aPos, aW, aH, aTr->getRot(),
				b.pos, static_cast<float>(b.width), static_cast<float>(b.height), b.rot)) {

				g->getAsteroidsUtils()->split_asteroid(a);
				b.used = false;
				break;
			}
		}
	}
}