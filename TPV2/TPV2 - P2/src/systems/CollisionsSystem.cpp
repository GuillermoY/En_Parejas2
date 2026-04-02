// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/MiracleFood.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../game/messages_defs.h"
#include "../utils/Collisions.h"

CollisionsSystem::CollisionsSystem() {}
CollisionsSystem::~CollisionsSystem() {}
void CollisionsSystem::initSystem() {}

void CollisionsSystem::update() {
	checkFoodCollisions();
	checkGhostCollisions();
}

void CollisionsSystem::checkFoodCollisions() {
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTR = _mngr->getComponent<Transform>(pm);

	auto& foods = _mngr->getEntities(ecs::grp::FOOD);
	auto  n = foods.size();

	for (auto i = 0u; i < n; i++) {
		auto e = foods[i];
		if (!_mngr->isAlive(e)) continue;

		auto eTR = _mngr->getComponent<Transform>(e);

		if (Collisions::collides(
			pmTR->_pos, pmTR->_width, pmTR->_height,
			eTR->_pos, eTR->_width, eTR->_height)) {

			bool miraculous = false;
			if (_mngr->hasComponent<MiracleFood>(e))
				miraculous = _mngr->getComponent<MiracleFood>(e)->isMiraculous();

			Message m;
			m.id = _m_PACMAN_FOOD_COLLISION;
			m.food_collision_data.e = e;
			m.food_collision_data.miraculous = miraculous;
			_mngr->send(m);
		}
	}
}

void CollisionsSystem::checkGhostCollisions() {
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTR = _mngr->getComponent<Transform>(pm);

	auto& ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	auto  n = ghosts.size();

	for (auto i = 0u; i < n; i++) {
		auto e = ghosts[i];
		if (!_mngr->isAlive(e)) continue;

		auto eTR = _mngr->getComponent<Transform>(e);

		if (Collisions::collides(
			pmTR->_pos, pmTR->_width, pmTR->_height,
			eTR->_pos, eTR->_width, eTR->_height)) {

			Message m;
			m.id = _m_PACMAN_GHOST_COLLISION;
			m.ghost_collision_data.e = e;
			_mngr->send(m);
			break; // solo una colisión por frame
		}
	}
}