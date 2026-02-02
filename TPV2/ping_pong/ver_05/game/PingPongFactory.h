// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

class GameObject;
class GameManager;

class PingPongFactory {
public:
	PingPongFactory() {
	}
	virtual ~PingPongFactory() {
	}
	virtual GameObject* createBall() = 0;
	virtual GameObject* createLeftPaddle(GameObject *ball) = 0;
	virtual GameObject* createRightPaddle(GameObject *ball) = 0;
	virtual GameManager* createGameManager(GameObject *ball,
			GameObject *leftPaddle, GameObject *rightPaddle) = 0;
};

