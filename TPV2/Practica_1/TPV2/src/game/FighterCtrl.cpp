#include "FighterCtrl.h"

#include "../sdlutils/InputHandler.h"
#include "Container.h"

FighterCtrl::FighterCtrl() {
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::handleInput(Container* o) {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_LEFT)) {
			r = o->getRotation() - 5.0f;
			o->setRotation(r);
		}
		else if (ihdlr.isKeyDown(SDLK_RIGHT)) {
			r = o->getRotation() + 5.0f;
			o->setRotation(r);
		}
		else if (ihdlr.isKeyDown(SDLK_UP)) {
			if (vel.getY() < speedLimit)
			{
				newVel = vel + Vector2D(0, -1).rotate(r) * thrust;
			}
			else
			{
				newVel = newVel.normalize()*speedLimit;
			}
			o->getVel().set(newVel);
		}
	}
}
