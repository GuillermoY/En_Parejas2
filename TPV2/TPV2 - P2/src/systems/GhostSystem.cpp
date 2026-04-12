// This file is part of the course TPV2@UCM - Samir Genaim

#include "GhostSystem.h"

#include "../components/FramedImage.h"
#include "../components/Health.h"
#include "../components/Immunity.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../game/messages_defs.h"
#include "../game/Game.h"
#include "../game/GameOverState.h"
#include "../game/NewRoundState.h"
#include "../sdlutils/SDLUtils.h"

constexpr int    SPR_FRAME_W = 128;
constexpr int    SPR_FRAME_H = 128;
constexpr int    SPR_COLS = 8;
constexpr Uint32 GHOST_FRAME_TIME = 120;

GhostSystem::GhostSystem() :
	_lastSpawnTime(0), _immunityActive(false), _currNumGhosts(0) {}
GhostSystem::~GhostSystem() {}
void GhostSystem::initSystem() {}

void GhostSystem::spawnGhost() {
	if (_currNumGhosts >= MAX_GHOSTS) return;

	auto& rand = sdlutils().rand();
	auto* tex = &sdlutils().images().at("pacman_sprites");

	float x, y;
	int corner = rand.nextInt(0, 3);
	switch (corner) {
	case 0: x = 0;                                y = 0;                                 break;
	case 1: x = sdlutils().width() - GHOST_SIZE;  y = 0;                                 break;
	case 2: x = 0;                                y = sdlutils().height() - GHOST_SIZE;  break;
	default:x = sdlutils().width() - GHOST_SIZE;  y = sdlutils().height() - GHOST_SIZE;  break;
	}

	auto e = _mngr->addEntity(ecs::grp::GHOSTS);
	auto tr = _mngr->addComponent<Transform>(e);
	tr->init(Vector2D(x, y), Vector2D(0, 0), GHOST_SIZE, GHOST_SIZE, 0.0f);

	int firstFrame = _immunityActive ? BLUE_FIRST_FRAME : RED_FIRST_FRAME;
	int numFrames = _immunityActive ? BLUE_NUM_FRAMES : RED_NUM_FRAMES;

	_mngr->addComponent<FramedImage>(e,
		tex, SPR_FRAME_W, SPR_FRAME_H, SPR_COLS,
		firstFrame, numFrames, GHOST_FRAME_TIME);

	_currNumGhosts++;
}

void GhostSystem::removeAllGhosts() {
	for (auto e : _mngr->getEntities(ecs::grp::GHOSTS))
		_mngr->setAlive(e, false);
	_currNumGhosts = 0;
}

void GhostSystem::setGhostsBlue() {
	for (auto e : _mngr->getEntities(ecs::grp::GHOSTS)) {
		if (_mngr->isAlive(e));
		_mngr->getComponent<FramedImage>(e)->setAnimation(BLUE_FIRST_FRAME, BLUE_NUM_FRAMES);
	}
}

void GhostSystem::setGhostsRed() {
	for (auto e : _mngr->getEntities(ecs::grp::GHOSTS)) {
		if (_mngr->isAlive(e));
		_mngr->getComponent<FramedImage>(e)->setAnimation(RED_FIRST_FRAME, RED_NUM_FRAMES);
	}
}

void GhostSystem::onGhostEaten(ecs::entity_t e) {
	_mngr->setAlive(e, false);
	_currNumGhosts--;
	sdlutils().soundEffects().at("pacman_eat").play("se");
}

void GhostSystem::onPacManHit() {
	sdlutils().soundEffects().at("pacman_death").play("se");

	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto health = _mngr->getComponent<Health>(pm);
	int lives = health->loseLife();

	removeAllGhosts();

	if (lives <= 0) {
		Message m;
		m.id = _m_GAME_OVER;
		m.game_over_data.won = false;
		_mngr->send(m);
	}
	else {
		Message m;
		m.id = _m_ROUND_OVER;
		_mngr->send(m);
		Game::instance()->setState(Game::instance()->getNewRoundState());
	}
}

void GhostSystem::update() {
	auto currTime = sdlutils().currRealTime();

	if (!_immunityActive && _currNumGhosts < MAX_GHOSTS && currTime - _lastSpawnTime >= SPAWN_INTERVAL) {
		spawnGhost();
		_lastSpawnTime = currTime;
	}

	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTR = _mngr->getComponent<Transform>(pm);
	auto& rand = sdlutils().rand();

	auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	auto n = ghosts.size();

	for (auto i = 0; i < n; i++) {
		auto e = ghosts[i];
		if (_mngr->isAlive(e)) {
			auto tr = _mngr->getComponent<Transform>(e);

			if (rand.nextInt(0, 999) < 5) {
				Vector2D dir = pmTR->_pos - tr->_pos;
				if (dir.magnitude() > 0)
					tr->_vel = dir.normalize() * GHOST_SPEED;
			}

			tr->_pos = tr->_pos + tr->_vel;

			if (tr->_pos.getX() < 0) {
				tr->_pos.setX(0.0f);
				tr->_vel.set(-tr->_vel.getX(), tr->_vel.getY());
			}
			else if (tr->_pos.getX() + tr->_width > sdlutils().width()) {
				tr->_pos.setX(sdlutils().width() - tr->_width);
				tr->_vel.set(-tr->_vel.getX(), tr->_vel.getY());
			}
			if (tr->_pos.getY() < 0) {
				tr->_pos.setY(0.0f);
				tr->_vel.set(tr->_vel.getX(), -tr->_vel.getY());
			}
			else if (tr->_pos.getY() + tr->_height > sdlutils().height()) {
				tr->_pos.setY(sdlutils().height() - tr->_height);
				tr->_vel.set(tr->_vel.getX(), -tr->_vel.getY());
			}

			_mngr->getComponent<FramedImage>(e)->update(currTime);
		}
	}
}

void GhostSystem::recieve(const Message& m) {
	switch (m.id) {
	case _m_NEW_GAME:
		removeAllGhosts();
		_lastSpawnTime = sdlutils().currRealTime();
		_immunityActive = false;
		break;
	case _m_ROUND_START:
		_lastSpawnTime = sdlutils().currRealTime();
		_immunityActive = false;
		break;
	case _m_ROUND_OVER:
		removeAllGhosts();
		break;
	case _m_IMMUNITY_START:
		_immunityActive = true;
		setGhostsBlue();
		break;
	case _m_IMMUNITY_END:
		_immunityActive = false;
		setGhostsRed();
		_lastSpawnTime = sdlutils().currRealTime();
		break;
	case _m_PACMAN_GHOST_COLLISION:
	{
		auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
		auto immunity = _mngr->getComponent<Immunity>(pm);
		if (immunity->_active)
			onGhostEaten(m.ghost_collision_data.e);
		else
			onPacManHit();
	}
	break;
	default:
		break;
	}
}