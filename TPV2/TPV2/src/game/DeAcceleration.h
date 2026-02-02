// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "PhysicsComponent.h"
#include <SDL.h>

class DeAcceleration : public PhysicsComponent {
public:
	DeAcceleration();
	virtual ~DeAcceleration();

	void update(Container* o) override;

private:

};

