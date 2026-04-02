// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include <SDL_rect.h>
#include "../components/Image.h"
#include "../components/RectangleViewer.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawMsgs();
	drawScore();
	drawBall();
	drawPaddles();
}

void RenderSystem::drawMsgs() {
	auto state = _mngr->getSystem<GameCtrlSystem>()->getState();

	// message when game is not running
	if (state != GameCtrlSystem::RUNNING) {

		// game over message
		if (state == GameCtrlSystem::GAMEOVER) {
			auto &t = sdlutils().msgs().at("gameover");
			t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
		}

		// new game message
		if (state == GameCtrlSystem::NEWGAME) {
			auto &t = sdlutils().msgs().at("start");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		} else {
			auto &t = sdlutils().msgs().at("continue");
			t.render((sdlutils().width() - t.width()) / 2,
					sdlutils().height() / 2 + t.height() * 2);
		}
	}
}

void RenderSystem::drawScore() {

	auto leftScore = _mngr->getSystem<GameCtrlSystem>()->getScore(0);
	auto rightScore = _mngr->getSystem<GameCtrlSystem>()->getScore(1);

	Texture scoreMsg(
			sdlutils().renderer(), //
			std::to_string(leftScore) + " - " + std::to_string(rightScore),
			sdlutils().fonts().at("ARIAL16"), build_sdlcolor(0xffffffff));
	scoreMsg.render((sdlutils().width() - scoreMsg.width()) / 2, 10);

}

void RenderSystem::drawBall() {
	auto ball = _mngr->getHandler(ecs::hdlr::BALL);
	auto ballTr = _mngr->getComponent<Transform>(ball);
	auto ballImg = _mngr->getComponent<Image>(ball);

	SDL_FRect dest = build_sdlfrect(ballTr->_pos, ballTr->_width,
			ballTr->_height);

	assert(ballImg->_tex != nullptr);
	ballImg->_tex->render(dest, ballTr->_rot);

}

void RenderSystem::drawPaddles() {
	for (auto e : _mngr->getEntities(ecs::grp::PADDLES)) {
		auto paddleTr_ = _mngr->getComponent<Transform>(e);

		SDL_FRect dest = build_sdlfrect(paddleTr_->_pos.getX(),
				paddleTr_->_pos.getY(), paddleTr_->_width, paddleTr_->_height);

		_mngr->getComponent<RectangleViewer>(e)->draw(sdlutils().renderer(),
				dest);
	}
}
