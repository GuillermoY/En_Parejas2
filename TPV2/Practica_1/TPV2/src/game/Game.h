// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


namespace ecs {
	class EntityManager;
}

class Entity;
class Transform;
class GameState;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	void checkCollisions();
	ecs::EntityManager* _mngr;

	Transform* _fighterTr;
	Transform* _ballTr;
	GameState* _gameState;
};

