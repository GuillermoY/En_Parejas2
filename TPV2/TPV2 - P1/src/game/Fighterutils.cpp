// This file is part of the course TPV2@UCM - Samir Genaim

#include "FighterUtils.h"

#include <cassert>
#include "../components/DeAcceleration.h"
#include "../components/FighterCtrl.h"
#include "../components/Gun.h"
#include "../components/Health.h"
#include "../components/Image.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "Game.h"

FighterUtils::FighterUtils() {
}

FighterUtils::~FighterUtils() {
}

void FighterUtils::create_fighter() {
	auto* mngr = Game::Instance()->getMngr();

	auto* fighter = mngr->addEntity();
	mngr->setHandler(ecs::hdlr::FIGHTER, fighter);

	float fw = 50.0f;
	float fh = 50.0f;
	float fx = (sdlutils().width() - fw) / 2.0f;
	float fy = (sdlutils().height() - fh) / 2.0f;

	auto* tr = fighter->addComponent<Transform>();
	tr->init(Vector2D(fx, fy), Vector2D(0.0f, 0.0f), fw, fh, 0.0f);

	fighter->addComponent<Image>(&sdlutils().images().at("fighter"));
	fighter->addComponent<DeAcceleration>();
	fighter->addComponent<Health>();
	fighter->addComponent<FighterCtrl>();
	fighter->addComponent<Gun>();
	fighter->addComponent<ShowAtOppositeSide>();
}

void FighterUtils::reset_fighter() {
	auto* mngr = Game::Instance()->getMngr();
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	assert(fighter != nullptr);

	// Recolocar en el centro
	auto* tr = fighter->getComponent<Transform>();
	assert(tr != nullptr);
	float fw = tr->getWidth();
	float fh = tr->getHeight();
	tr->init(
		Vector2D((sdlutils().width() - fw) / 2.0f,
			(sdlutils().height() - fh) / 2.0f),
		Vector2D(0.0f, 0.0f),
		fw, fh, 0.0f
	);

	// Desactivar balas activas
	auto* gun = fighter->getComponent<Gun>();
	assert(gun != nullptr);
	gun->reset();
}

void FighterUtils::reset_lives() {
	auto* mngr = Game::Instance()->getMngr();
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	assert(fighter != nullptr);

	auto* health = fighter->getComponent<Health>();
	assert(health != nullptr);
	health->resetLives();
}

int FighterUtils::update_lives(int n) {
	auto* mngr = Game::Instance()->getMngr();
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	assert(fighter != nullptr);

	auto* health = fighter->getComponent<Health>();
	assert(health != nullptr);

	if (n < 0) {
		return health->loseLife();
	}
	// si n > 0 podría añadir vidas, pero por ahora solo usamos -1
	return health->getLives();
}