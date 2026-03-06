// This file is part of the course TPV2@UCM - Samir Genaim

#include "DeAcceleration.h"

void DeAcceleration::update() {
	getVel().set(getVel() * 0.995f);
}