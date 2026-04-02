// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

namespace ecs {
class EntityManager;
}

class BallSystem;
class PaddlesSystem;
class GameCtrlSystem;
class CollisionsSystem;
class RenderSystem;


class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	ecs::EntityManager *_mngr;

	BallSystem *_ballSys;
	PaddlesSystem *_paddlesSys;
	GameCtrlSystem *_gameCtrlSys;
	CollisionsSystem *_collisionsSys;
	RenderSystem *_renderSys;
};

