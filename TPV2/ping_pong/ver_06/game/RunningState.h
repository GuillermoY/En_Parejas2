// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "GameState.h"

class GameManager;
class GameObject;

class RunningState: public GameState {
public:
	RunningState();
	virtual ~RunningState();
	void leave() override;
	void update() override;
	void enter() override;

private:
	void checkCollisions();

	GameManager *_gm;
	GameObject *_leftPaddle;
	GameObject *_rightPaddle;
	GameObject *_ball;
};

