// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "PhysicsComponent.h"
#include <SDL.h>

class ShowAtOpposieSide : public PhysicsComponent {
public:
	ShowAtOpposieSide();
	virtual ~ShowAtOpposieSide();

	void update(Container* o) override;

private:

};

#pragma once
