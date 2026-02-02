// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

#include "GameManager.h"

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Container;
class GameManager;
class PingPongFactory;

class Game {
public:
	Game();
	virtual ~Game();
	void init(PingPongFactory &facotry);
	void start();
private:
	void checkCollisions();

	GameManager *_gm;
	GameObject *_leftPaddle;
	GameObject *_rightPaddle;
	GameObject *_ball;
	std::vector<GameObject*> _objs;
};

