// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "Paddle.h"

class PaddleWithMouse: public Paddle {
public:
	PaddleWithMouse(SDL_Color color);
	virtual ~PaddleWithMouse();
	void handleInput() override;
};

