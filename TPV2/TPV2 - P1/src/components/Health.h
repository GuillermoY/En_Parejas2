// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Health : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::HEALTH)

		Health();
	virtual ~Health();

	void initComponent() override;
	void render() override;

	// Quita una vida. Devuelve las vidas restantes.
	int loseLife();

	// Resetea las vidas al valor inicial (3).
	void resetLives();

	// Devuelve el número de vidas actual.
	int getLives() const;

private:
	int _lives;
	static constexpr int _INITIAL_LIVES = 3;
};
