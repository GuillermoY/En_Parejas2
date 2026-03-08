// This file is part of the course TPV2@UCM - Samir Genaim

#include "AsteroidsUtils.h"

#include <cassert>
#include <algorithm>
#include "../components/Follow.h"
#include "../components/Generations.h"
#include "../components/ImageWithFrames.h"
#include "../components/MaterialConsistency.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/TeleportOnExit.h"
#include "../components/TowardDestination.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "Game.h"

AsteroidsUtils::AsteroidsUtils() {
}

AsteroidsUtils::~AsteroidsUtils() {
}

// ---------------------------------------------------------------------------
// Crea un asteroide en un borde aleatorio con velocidad hacia el centro
// ---------------------------------------------------------------------------
ecs::Entity* AsteroidsUtils::create_one_asteroid(int generations, bool hasMaterial) {
	auto* mngr = Game::Instance()->getMngr();
	auto& r = sdlutils().rand();

	// Tamaño según generación
	float size = 30.0f + 15.0f * static_cast<float>(generations);

	// Posición aleatoria en los bordes
	int sw = sdlutils().width();
	int sh = sdlutils().height();
	float px, py;
	int border = r.nextInt(0, 4);
	switch (border) {
	case 0: // top
		px = static_cast<float>(r.nextInt(0, sw));
		py = 0.0f;
		break;
	case 1: // bottom
		px = static_cast<float>(r.nextInt(0, sw));
		py = static_cast<float>(sh) - size;
		break;
	case 2: // left
		px = 0.0f;
		py = static_cast<float>(r.nextInt(0, sh));
		break;
	default: // right
		px = static_cast<float>(sw) - size;
		py = static_cast<float>(r.nextInt(0, sh));
		break;
	}

	// Destino aleatorio cerca del centro
	float cx = sw / 2.0f + static_cast<float>(r.nextInt(-100, 101));
	float cy = sh / 2.0f + static_cast<float>(r.nextInt(-100, 101));
	float speed = r.nextInt(1, 10) / 10.0f;
	Vector2D pos(px, py);
	Vector2D dest(cx, cy);
	Vector2D vel = (dest - pos).normalize() * speed;

	// Crear entidad
	auto* asteroid = mngr->addEntity(ecs::grp::ASTEROIDS);

	auto* tr = asteroid->addComponent<Transform>();
	tr->init(pos, vel, size, size, 0.0f);

	// Imagen con frames (asteroid.png tiene 8 frames de 64x64)
	bool useGold = r.nextInt(0, 2) == 0;
	if (useGold) {
		asteroid->addComponent<ImageWithFrames>(
			&sdlutils().images().at("asteroid_gold"), 30, 85, 100, 6);
	}
	else {
		asteroid->addComponent<ImageWithFrames>(
			&sdlutils().images().at("asteroid"), 30, 85, 100, 6);
	}

	asteroid->addComponent<Generations>(generations);

	// ShowAtOppositeSide o TeleportOnExit (aleatorio)
	if (r.nextInt(0, 2) == 0) {
		asteroid->addComponent<ShowAtOppositeSide>();
	}
	else {
		asteroid->addComponent<TeleportOnExit>();
	}

	// Follow, TowardDestination o ninguno (aleatorio)
	int behavior = r.nextInt(0, 3);
	if (behavior == 0) {
		asteroid->addComponent<Follow>();
	}
	else if (behavior == 1) {
		asteroid->addComponent<TowardDestination>();
	}

	// MaterialConsistency (aleatorio)
	if (hasMaterial) {
		asteroid->addComponent<MaterialConsistency>();
	}

	return asteroid;
}

void AsteroidsUtils::create_asteroids(int n) {
	auto& r = sdlutils().rand();
	for (int i = 0; i < n; i++) {
		int  gen = r.nextInt(1, 4);        // 1-3
		bool hasMaterial = r.nextInt(0, 2) == 0;   // 50% probabilidad
		create_one_asteroid(gen, hasMaterial);
	}
}

void AsteroidsUtils::remove_all_asteroids() {
	auto* mngr = Game::Instance()->getMngr();
	for (auto* e : mngr->getEntities(ecs::grp::ASTEROIDS)) {
		e->setAlive(false);
	}
	mngr->refresh();
}

void AsteroidsUtils::split_asteroid(ecs::Entity* a) {
	assert(a != nullptr);

	auto* mngr = Game::Instance()->getMngr();
	auto& r = sdlutils().rand();

	auto* tr = a->getComponent<Transform>();
	auto* gen = a->getComponent<Generations>();
	assert(tr != nullptr && gen != nullptr);

	int  parentGen = gen->getGenerations();
	bool hasMaterial = a->hasComponent<MaterialConsistency>();
	int  matConsist = hasMaterial ?
		a->getComponent<MaterialConsistency>()->getConsistency() : 0;

	// Desactivar el asteroide original
	a->setAlive(false);
	sdlutils().soundEffects().at("explosion").play("se");

	// Si no quedan generaciones, no se divide
	if (parentGen <= 0) return;

	int newGen = parentGen - 1;
	float pw = tr->getWidth();
	float ph = tr->getHeight();
	Vector2D pp = tr->getPos();
	Vector2D pv = tr->getVel();
	float newSize = 30.0f + 15.0f * static_cast<float>(newGen);

	for (int i = 0; i < 2; i++) {
		float rotDeg = static_cast<float>(r.nextInt(0, 360));
		Vector2D pos = pp + pv.rotate(rotDeg) * 2.0f * std::max(pw, ph);
		Vector2D vel = pv.rotate(rotDeg) * 1.1f;

		auto* child = mngr->addEntity(ecs::grp::ASTEROIDS);

		auto* childTr = child->addComponent<Transform>();
		childTr->init(pos, vel, newSize, newSize, 0.0f);

		bool useGold = r.nextInt(0, 2) == 0;
		if (useGold) {
			child->addComponent<ImageWithFrames>(
				&sdlutils().images().at("asteroid_gold"), 30, 85, 100, 6);
		}
		else {
			child->addComponent<ImageWithFrames>(
				&sdlutils().images().at("asteroid"), 30, 85, 100, 6);
		}

		child->addComponent<Generations>(newGen);

		if (r.nextInt(0, 2) == 0) {
			child->addComponent<ShowAtOppositeSide>();
		}
		else {
			child->addComponent<TeleportOnExit>();
		}

		int behavior = r.nextInt(0, 3);
		if (behavior == 0) {
			child->addComponent<Follow>();
		}
		else if (behavior == 1) {
			child->addComponent<TowardDestination>();
		}

		if (hasMaterial) {
			child->addComponent<MaterialConsistency>(matConsist);
		}
	}
}