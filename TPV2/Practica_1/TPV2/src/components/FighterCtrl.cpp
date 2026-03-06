#include "FighterCtrl.h"


void FighterCtrl::handleInput() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_LEFT)) {
			r = o->getRot() - 5.0f;
			o->setRot(r);
		}
		else if (ihdlr.isKeyDown(SDLK_RIGHT)) {
			r = o->getRot() + 5.0f;
			o->setRot(r);
		}
		else if (ihdlr.isKeyDown(SDLK_UP)) {
			sdlutils().soundEffects().at("paddle_hit").play("se");
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
