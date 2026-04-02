// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cstdint>
#include "../ecs/ecs.h"

using msgId_t = uint8_t;

enum msgId : msgId_t {
	_m_NEW_GAME,
	_m_ROUND_START,
	_m_ROUND_OVER,
	_m_GAME_OVER,
	_m_PACMAN_FOOD_COLLISION,
	_m_PACMAN_GHOST_COLLISION,
	_m_IMMUNITY_START,
	_m_IMMUNITY_END
};

struct Message {
	msgId_t id;

	union {
		struct {
			ecs::entity_t e;
			bool miraculous;
		} food_collision_data;

		struct {
			ecs::entity_t e;
		} ghost_collision_data;

		struct {
			bool won;
		} game_over_data;
	};
};