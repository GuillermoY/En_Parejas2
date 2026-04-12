// This file is part of the course TPV2@UCM - Samir Genaim

#include "FoodSystem.h"

#include "../components/FramedImage.h"
#include "../components/MiracleFood.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../game/messages_defs.h"
#include "../game/Game.h"
#include "../game/GameOverState.h"
#include "../sdlutils/SDLUtils.h"

constexpr int   SPR_FRAME_W = 128;
constexpr int   SPR_FRAME_H = 128;
constexpr int   SPR_COLS = 8;
constexpr int   CHERRY_FRAME = 12;  // row 1, col 4
constexpr int   PEAR_FRAME = 15;  // row 1, col 7

constexpr int   FOOD_COLS = 10;
constexpr int   FOOD_ROWS = 6;
constexpr float FOOD_SIZE = 30.0f;
constexpr float FOOD_PAD_X = 60.0f;
constexpr float FOOD_PAD_Y = 80.0f;
constexpr float MIRACLE_PROB = 0.10f;

FoodSystem::FoodSystem() : _totalFood(0), _eatenFood(0) {}
FoodSystem::~FoodSystem() {}
void FoodSystem::initSystem() {}

void FoodSystem::createFoodGrid() {
	auto& rand = sdlutils().rand();
	auto* tex = &sdlutils().images().at("pacman_sprites");

	float stepX = (sdlutils().width() - 2 * FOOD_PAD_X) / (FOOD_COLS - 1);
	float stepY = (sdlutils().height() - 2 * FOOD_PAD_Y) / (FOOD_ROWS - 1);

	_totalFood = FOOD_COLS * FOOD_ROWS;
	_eatenFood = 0;

	for (int r = 0; r < FOOD_ROWS; r++) {
		for (int c = 0; c < FOOD_COLS; c++) {
			float x = FOOD_PAD_X + c * stepX - FOOD_SIZE / 2.0f;
			float y = FOOD_PAD_Y + r * stepY - FOOD_SIZE / 2.0f;

			auto e = _mngr->addEntity(ecs::grp::FOOD);
			auto tr = _mngr->addComponent<Transform>(e);
			tr->init(Vector2D(x, y), Vector2D(), FOOD_SIZE, FOOD_SIZE, 0.0f);

			bool miracle = (rand.nextInt(0, 99) < 10);

			_mngr->addComponent<FramedImage>(e,
				tex, SPR_FRAME_W, SPR_FRAME_H, SPR_COLS,
				CHERRY_FRAME, 1, 0);

			if (miracle) {
				int N_ms = rand.nextInt(10, 20) * 1000;
				auto* mf = _mngr->addComponent<MiracleFood>(e);
				mf->init(N_ms, sdlutils().currRealTime());
			}
		}
	}
}

void FoodSystem::removeAllFood() {
	for (auto e : _mngr->getEntities(ecs::grp::FOOD))
		_mngr->setAlive(e, false);
	_totalFood = 0;
	_eatenFood = 0;
}

void FoodSystem::update() {
	auto currTime = sdlutils().currRealTime();
	auto& rand = sdlutils().rand();

	auto& foods = _mngr->getEntities(ecs::grp::FOOD);
	auto n = foods.size();

	for (auto i = 0; i < n; i++) {
		auto e = foods[i];
		if (_mngr->isAlive(e)) {
			if (_mngr->hasComponent<MiracleFood>(e)) {
				auto* mf = _mngr->getComponent<MiracleFood>(e);
				auto* fi = _mngr->getComponent<FramedImage>(e);

				if (mf->_state == MiracleFood::NORMAL) {
					if (currTime - mf->_stateStartTime >= mf->_N) {
						mf->_state = MiracleFood::MIRACULOUS;
						mf->_stateStartTime = currTime;
						mf->_M = rand.nextInt(1, 5) * 1000;
						fi->setAnimation(PEAR_FRAME, 1);
					}
				}
				else {
					if (currTime - mf->_stateStartTime >= mf->_M) {
						mf->_state = MiracleFood::NORMAL;
						mf->_stateStartTime = currTime;
						fi->setAnimation(CHERRY_FRAME, 1);
					}
				}
			}
		}
	}
}

void FoodSystem::onFoodEaten(ecs::entity_t e) {
	_mngr->setAlive(e, false);
	_eatenFood++;

	sdlutils().soundEffects().at("pacman_eat").play("se");

	if (_eatenFood >= _totalFood) {
		sdlutils().soundEffects().at("pacman_won").play("se");
		Message m;
		m.id = _m_GAME_OVER;
		m.game_over_data.won = true;
		_mngr->send(m);
	}
}

void FoodSystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_NEW_GAME:
		removeAllFood();
		createFoodGrid();
		break;
	case _m_ROUND_START:
	{
		auto currTime = sdlutils().currRealTime();
		for (auto e : _mngr->getEntities(ecs::grp::FOOD)) {
			if (!_mngr->isAlive(e)) continue;
			if (!_mngr->hasComponent<MiracleFood>(e)) continue;
			auto* mf = _mngr->getComponent<MiracleFood>(e);
			mf->_state = MiracleFood::NORMAL;
			mf->_stateStartTime = currTime;
			_mngr->getComponent<FramedImage>(e)->setAnimation(CHERRY_FRAME, 1);
		}
	}
	break;
	case _m_PACMAN_FOOD_COLLISION:
		onFoodEaten(m.food_collision_data.e);
		break;
	case _m_GAME_OVER:
	{
		auto* gs = Game::instance()->getGameOverState();
		gs->setWon(m.game_over_data.won);
		Game::instance()->setState(gs);
	}
	break;
	default:
		break;
	}
}