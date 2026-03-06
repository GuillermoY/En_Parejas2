// This file is part of the course TPV2@UCM - Samir Genaim

#include "ShowAtOpposieSide.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

void ShowAtOpposieSide::update() {
	if (sdlutils().height() < this->getPos().getY())
	{
		this->getPos().setY(0.0 - this->getHeight());
	}
	else if (0 - this->getHeight() > this->getPos().getY())
	{
		this->getPos().setY(sdlutils().height());
	}

	if (sdlutils().width() < this->getPos().getX())
	{
		this->getPos().setX(0.0 - this->getWidth());
	}
	else if (0 - this->getWidth() > this->getPos().getX())
	{
		this->getPos().setX(sdlutils().width());
	}
}



