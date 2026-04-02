// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/FramedImage.h"
#include "../components/Health.h"
#include "../components/Immunity.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/messages_defs.h"

constexpr int    SPR_FRAME_W = 128;
constexpr int    SPR_FRAME_H = 128;
constexpr int    SPR_COLS = 8;
constexpr int    PM_FIRST_FRAME = 0;
constexpr int    PM_NUM_FRAMES = 4;
constexpr Uint32 PM_FRAME_TIME = 80;

PacManSystem::PacManSystem() : _pmTR(nullptr) {}
PacManSystem::~PacManSystem() {}

void PacManSystem::initSystem() {
	auto pacman = _mngr->addEntity();
	_mngr->setHandler(ecs::hdlr::PACMAN, pacman);

	constexpr float SIZE = 40.0f;
	float x = (sdlutils().width() - SIZE) / 2.0f;
	float y = (sdlutils().height() - SIZE) / 2.0f;

	_pmTR = _mngr->addComponent<Transform>(pacman);
	_pmTR->init(Vector2D(x, y), Vector2D(0, 0), SIZE, SIZE, 0.0f);

	auto* tex = &sdlutils().images().at("pacman_sprites");
	_mngr->addComponent<FramedImage>(pacman,
		tex, SPR_FRAME_W, SPR_FRAME_H, SPR_COLS,
		PM_FIRST_FRAME, PM_NUM_FRAMES, PM_FRAME_TIME);

	_mngr->addComponent<Health>(pacman);
	_mngr->addComponent<Immunity>(pacman);
}

void PacManSystem::resetPosition() {
	constexpr float SIZE = 40.0f;
	float x = (sdlutils().width() - SIZE) / 2.0f;
	float y = (sdlutils().height() - SIZE) / 2.0f;
	_pmTR->_pos = Vector2D(x, y);
	_pmTR->_vel = Vector2D(0, 0);
	_pmTR->_rot = 0.0f;
}

void PacManSystem::resetLives() {
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	_mngr->getComponent<Health>(pm)->reset();
}

void PacManSystem::update() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_RIGHT)) {
			_pmTR->_rot += 90.0f;
			_pmTR->_vel = _pmTR->_vel.rotate(90.0f);
		}
		else if (ihdlr.isKeyDown(SDL_SCANCODE_LEFT)) {
			_pmTR->_rot -= 90.0f;
			_pmTR->_vel = _pmTR->_vel.rotate(-90.0f);
		}
		else if (ihdlr.isKeyDown(SDL_SCANCODE_UP)) {
			_pmTR->_vel = Vector2D(0.0f, -3.0f).rotate(_pmTR->_rot);
			sdlutils().soundEffects().at("pacman_chomp").play("se");
		}
		else if (ihdlr.isKeyDown(SDL_SCANCODE_DOWN)) {
			_pmTR->_vel = Vector2D(0.0f, 0.0f);
		}
	}

	_pmTR->_pos = _pmTR->_pos + _pmTR->_vel;

	if (_pmTR->_pos.getX() < 0) {
		_pmTR->_pos.setX(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getX() + _pmTR->_width > sdlutils().width()) {
		_pmTR->_pos.setX(sdlutils().width() - _pmTR->_width);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	if (_pmTR->_pos.getY() < 0) {
		_pmTR->_pos.setY(0.0f);
		_pmTR->_vel.set(0.0f, 0.0f);
	}
	else if (_pmTR->_pos.getY() + _pmTR->_height > sdlutils().height()) {
		_pmTR->_pos.setY(sdlutils().height() - _pmTR->_height);
		_pmTR->_vel.set(0.0f, 0.0f);
	}

	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	_mngr->getComponent<FramedImage>(pm)->update(sdlutils().currRealTime());
}

void PacManSystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_NEW_GAME:
		resetLives();
		resetPosition();
		break;
	case _m_ROUND_START:
		resetPosition();
		{
			auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
			_mngr->getComponent<Immunity>(pm)->deactivate();
		}
		break;
	default:
		break;
	}
}