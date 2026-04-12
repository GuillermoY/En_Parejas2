// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "../ecs/System.h"

class GhostSystem : public ecs::System {
public:
	GhostSystem();
	virtual ~GhostSystem();

	void initSystem() override;
	void update()     override;
	void recieve(const Message& m) override;

private:
	static constexpr int    MAX_GHOSTS = 10;
	static constexpr Uint32 SPAWN_INTERVAL = 5000;
	static constexpr float  GHOST_SIZE = 40.0f;
	static constexpr float  CHASE_PROB = 0.005f;
	static constexpr float  GHOST_SPEED = 1.1f;

	static constexpr int RED_FIRST_FRAME = 32; // fila 4, col 0
	static constexpr int RED_NUM_FRAMES = 8;
	static constexpr int BLUE_FIRST_FRAME = 30; // fila 3, col 6
	static constexpr int BLUE_NUM_FRAMES = 2;

	Uint32 _lastSpawnTime;
	bool   _immunityActive;
	int    _currNumGhosts;

	void spawnGhost();
	void removeAllGhosts();
	void setGhostsBlue();
	void setGhostsRed();
	void onGhostEaten(ecs::entity_t e);
	void onPacManHit();
};