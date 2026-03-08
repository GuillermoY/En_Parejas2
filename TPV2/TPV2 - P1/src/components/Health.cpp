// This file is part of the course TPV2@UCM - Samir Genaim

#include "Health.h"

#include <cassert>
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

Health::Health() :
	_lives(_INITIAL_LIVES) {
}

Health::~Health() {
}

void Health::initComponent() {
	// no dependencies needed
}

void Health::render() {
	// Dibuja los corazones en la esquina superior izquierda
	auto& heartTex = sdlutils().images().at("heart");
	constexpr float scale = 0.5f;
	int w = heartTex.width() * scale;
	int h = heartTex.height() * scale;
	for (int i = 0; i < _lives; i++) {
		heartTex.render(SDL_FRect{ 10.0f + i * (w + 4), 10.0f, (float)w, (float)h });
	}
}

int Health::loseLife() {
	assert(_lives > 0);
	_lives--;
	return _lives;
}

void Health::resetLives() {
	_lives = _INITIAL_LIVES;
}

int Health::getLives() const {
	return _lives;
}