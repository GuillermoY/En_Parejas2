// This file is part of the course TPV2@UCM - Samir Genaim

#include "PauseState.h"

#include <cmath>
#include <limits>
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/macros.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"
#include "Game.h"

PauseState::PauseState() {
}

PauseState::~PauseState() {
}

void PauseState::enter() {
	sdlutils().virtualTimer().pause();
}

void PauseState::leave() {
	sdlutils().virtualTimer().resume();
}

void PauseState::update() {
	auto* mngr = Game::Instance()->getMngr();
	auto* fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	auto* fTr = fighter->getComponent<Transform>();
	auto* rend = sdlutils().renderer();
	auto& font = sdlutils().fonts().at("ARIAL16");

	// Calcular estadísticas
	auto& asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);
	int   numAsteroids = static_cast<int>(asteroids.size());

	// Distancia mínima caza-asteroides
	float minDist = std::numeric_limits<float>::max();
	Vector2D fCenter = fTr->getPos() + Vector2D(fTr->getWidth() / 2.0f, fTr->getHeight() / 2.0f);
	for (auto* a : asteroids) {
		if (!a->isAlive()) continue;
		auto* aTr = a->getComponent<Transform>();
		Vector2D aCenter = aTr->getPos() + Vector2D(aTr->getWidth() / 2.0f, aTr->getHeight() / 2.0f);
		float dist = (fCenter - aCenter).magnitude();
		if (dist < minDist) minDist = dist;
	}
	if (minDist == std::numeric_limits<float>::max()) minDist = 0.0f;

	// Mostrar vidas
	fighter->getComponent<Health>()->render();

	// Mostrar estadísticas
	float cx = sdlutils().width() / 2.0f;
	float cy = sdlutils().height() / 2.0f;

	Texture asteroidsMsg(rend,
		"Asteroids: " + std::to_string(numAsteroids),
		font, build_sdlcolor(0xffffffff));
	asteroidsMsg.render(cx - asteroidsMsg.width() / 2.0f, cy - 60.0f);

	char distStr[64];
	SDL_snprintf(distStr, sizeof(distStr), "Min distance to asteroid: %.1f", minDist);
	Texture distMsg(rend, distStr, font, build_sdlcolor(0xffffffff));
	distMsg.render(cx - distMsg.width() / 2.0f, cy - 30.0f);

	// Mensaje de pausa
	auto& t = sdlutils().msgs().at("paused");
	t.render(cx - t.width() / 2.0f, cy + 10.0f);

	// Cualquier tecla reanuda
	if (ih().keyDownEvent()) {
		Game::Instance()->setState(Game::RUNNING);
	}
}