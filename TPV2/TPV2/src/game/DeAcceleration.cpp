// This file is part of the course TPV2@UCM - Samir Genaim

#include "DeAcceleration.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"

void DeAcceleration::update(Container* o) {
	o->getVel().set(o->getVel() * 0.995f);
}



