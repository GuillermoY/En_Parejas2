// This file is part of the course TPV2@UCM - Samir Genaim

#include "ShowAtOpposieSide.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Container.h"

void ShowAtOpposieSide::update(Container* o) {
	if (sdlutils().height() < o->getPos().getY())
	{
		o->getPos().setY(0.0 - o->getHeight());
	}
	else if (0 - o->getHeight() > o->getPos().getY())
	{
		o->getPos().setY(sdlutils().height());
	}

	if (sdlutils().width() < o->getPos().getX())
	{
		o->getPos().setX(0.0 - o->getWidth());
	}
	else if (0 - o->getWidth() > o->getPos().getX())
	{
		o->getPos().setX(sdlutils().width());
	}
}



