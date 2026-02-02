// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "Paddle.h"

class PaddleNoFill: public Paddle {
public:
	PaddleNoFill(SDL_Color color);
	virtual ~PaddleNoFill();
	void render() override;
};
