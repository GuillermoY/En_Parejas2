// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include "../components/FramedImage.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

constexpr float LIFE_ICON_SIZE = 24.0f;
constexpr float LIFE_ICON_PAD = 4.0f;

RenderSystem::RenderSystem() {}
RenderSystem::~RenderSystem() {}
void RenderSystem::initSystem() {}

void RenderSystem::update() {
	sdlutils().clearRenderer();
	drawFood();
	drawGhosts();
	drawPacMan();
	drawLives();
	sdlutils().presentRenderer();
}

void RenderSystem::drawFood() {
	for (auto e : _mngr->getEntities(ecs::grp::FOOD)) {
		if (!_mngr->isAlive(e)) continue;
		drawFramed(
			_mngr->getComponent<Transform>(e),
			_mngr->getComponent<FramedImage>(e)
		);
	}
}

void RenderSystem::drawGhosts() {
	for (auto e : _mngr->getEntities(ecs::grp::GHOSTS)) {
		if (!_mngr->isAlive(e)) continue;
		drawFramed(
			_mngr->getComponent<Transform>(e),
			_mngr->getComponent<FramedImage>(e)
		);
	}
}

void RenderSystem::drawPacMan() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	drawFramed(
		_mngr->getComponent<Transform>(e),
		_mngr->getComponent<FramedImage>(e)
	);
}

void RenderSystem::drawLives() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto health = _mngr->getComponent<Health>(e);
	auto fi = _mngr->getComponent<FramedImage>(e);

	for (int i = 0; i < health->_lives; i++) {
		SDL_FRect src = {
			0.0f, 0.0f,
			static_cast<float>(fi->_frameW),
			static_cast<float>(fi->_frameH)
		};
		SDL_FRect dest = {
			10.0f + i * (LIFE_ICON_SIZE + LIFE_ICON_PAD),
			10.0f,
			LIFE_ICON_SIZE,
			LIFE_ICON_SIZE
		};
		assert(fi->_tex != nullptr);
		fi->_tex->render(src, dest, 0.0f);
	}
}

void RenderSystem::drawFramed(Transform* tr, FramedImage* fi) {
	SDL_Rect srcInt = fi->currentClip();
	SDL_FRect src = {
		static_cast<float>(srcInt.x),
		static_cast<float>(srcInt.y),
		static_cast<float>(srcInt.w),
		static_cast<float>(srcInt.h)
	};
	SDL_FRect dest = build_sdlfrect(tr->_pos, tr->_width, tr->_height);
	assert(fi->_tex != nullptr);
	fi->_tex->render(src, dest, tr->_rot);
}