// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <cstdint>
#include <SDL.h>
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

class Transform;

class Gun : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::GUN)

		struct Bullet {
		bool     used = false;
		Vector2D pos;
		Vector2D vel;
		int      width = 0;
		int      height = 0;
		float    rot = 0.0f;
	};

	constexpr static uint8_t _max_bullets = 20;
	typedef std::array<Bullet, _max_bullets> bullets_array_t;
	typedef bullets_array_t::iterator        iterator;

	Gun();
	virtual ~Gun();

	void initComponent() override;
	void update() override;
	void render() override;

	// Marca todas las balas como no usadas
	void reset();

	iterator begin() { return _bullets.begin(); }
	iterator end() { return _bullets.end(); }

private:
	// Añade una bala al pool desde la posición/vel del caza
	void shoot(Vector2D p, Vector2D v, int width, int height, float r);

	Transform* _tr;
	bullets_array_t    _bullets;
	uint8_t            _lastUsed;   // índice circular para búsqueda eficiente
	uint64_t           _lastShotTime;

	static constexpr float  _SHOT_INTERVAL = 250.0f;  // ms entre disparos
};