// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class GameObject;
class GameManager;
class Paddle;
class Ball;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	void checkCollisions();

	GameManager *_gm;
	Paddle *_leftPaddle;
	Paddle *_rightPaddle;
	Ball *_ball;
	std::vector<GameObject*> _objs;
};

