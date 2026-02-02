// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "Paddle.h"

class PaddleWithAI: public Paddle {
public:
	PaddleWithAI(SDL_Color color, GameObject *ball);
	virtual ~PaddleWithAI();
	void handleInput() override;
	void update() override;

private:
	bool ballComingTowardsPaddle();
	GameObject *_ball;
};

