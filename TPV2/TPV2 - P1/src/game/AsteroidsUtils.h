// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "AsteroidsFacade.h"

class AsteroidsUtils : public AsteroidsFacade {
public:
	AsteroidsUtils();
	virtual ~AsteroidsUtils();

	void create_asteroids(int n) override;
	void remove_all_asteroids() override;
	void split_asteroid(ecs::Entity *a) override;

private:
	// Crea un único asteroide con posición y velocidad dadas
	ecs::Entity* create_one_asteroid(int generations, bool hasMaterial);
};
