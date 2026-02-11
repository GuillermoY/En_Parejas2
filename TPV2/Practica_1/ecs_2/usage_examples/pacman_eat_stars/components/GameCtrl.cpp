// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrl.h"

#include <algorithm>

#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Image.h"
#include "StarMotion.h"
#include "Transform.h"

GameCtrl::GameCtrl() :
		_currNumOfStars(0), //
		_score(0), //
		_starsLimit(30) {
}

GameCtrl::~GameCtrl() {
}

void GameCtrl::initComponent() {
}

void GameCtrl::update() {
	auto &ihldr = ih();

	if (ihldr.keyDownEvent()) {
		if (ihldr.isKeyDown(SDL_SCANCODE_SPACE)) { // create start
			createStart(std::min(5u, _starsLimit - _currNumOfStars));
		}
	}
}

void GameCtrl::render() {

	// draw the score
	//
	Texture scoreTex(sdlutils().renderer(), std::to_string(_score),
			sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	SDL_FRect dest = build_sdlfrect( //
			(sdlutils().width() - scoreTex.width()) / 2.0f, //
			10.0f, //
			scoreTex.width(), //
			scoreTex.height());

	scoreTex.render(dest);

	// draw add stars message
	sdlutils().msgs().at("addstars").render(10, 10);
}

void GameCtrl::createStart(unsigned int n) {

	for (auto i = 0u; i < n; i++) {
		// Always use the random number generator provided by SDLUtils
		//
		auto &rand = sdlutils().rand();

		// add and entity to the manager
		//
		auto mngr = _ent->getMngr();
		auto e = mngr->addEntity(ecs::grp::STARS);

		// add a Transform component, and initialise it with random
		// size and position
		//
		auto tr = e->addComponent<Transform>();
		auto s = rand.nextInt(50, 100);
		auto x = rand.nextInt(0, sdlutils().width() - s);
		auto y = rand.nextInt(0, sdlutils().height() - s);
		tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);

		// add an Image Component
		//
		e->addComponent < Image > (&sdlutils().images().at("star"));

		// add a StarMotion component to resize/rotate the star
		//
		e->addComponent<StarMotion>();

		_currNumOfStars++;
	}
}
