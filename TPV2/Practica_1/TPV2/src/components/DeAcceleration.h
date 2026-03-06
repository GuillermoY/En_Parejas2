// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL.h>
#include "Transform.h"
class Transform;
class DeAcceleration : public ecs::Component {
public:
	DeAcceleration();
	virtual ~DeAcceleration();
	Transform trs;
	void update() override;

private:

};

