// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "PingPongFactory.h"

class AIPingPongFactory: public PingPongFactory {
public:
	AIPingPongFactory();
	virtual ~AIPingPongFactory();
	GameObject* createRightPaddle(GameObject *ball) override;
	GameObject* createLeftPaddle(GameObject *ball) override;
	GameManager* createGameManager(GameObject *ball, GameObject *leftPaddle,
			GameObject *rightPaddle) override;
	GameObject* createBall() override;
};

