#pragma once
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreInput.h>
#include <OgreMath.h>
#include <OgreFrameListener.h>
#include <OgreMeshManager.h>
#include <SDL_keycode.h>
#include <iostream>
#include "Character.h"

class Simbad : public Character {
public:
	Simbad(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: Character(initPos, node, sceneMng, mesh) {
		
		charactAnim = entity->getAnimationState("RunBase");
		charactAnim->setEnabled(true);
		charactAnim->setLoop(true);
	};

	int getRemainingLives() const { return lives; }
	int getPoints() const { return points; }
	bool isInvulnerable() const { return invulnerableTime > 0.0f; }

	void hit()
	{
		if (isInvulnerable() || lives == 0)
			return;
		lives--;
		invulnerableTime = INVULNERABLE_DURATION;
	}

	void update(Real dt) override
	{
		Character::update(dt);
		if (invulnerableTime > 0.0f)
			invulnerableTime -= dt;
	}

private:
	static constexpr int INITIAL_LIVES = 3;
	static constexpr float INVULNERABLE_DURATION = 2.0f;

	int lives = INITIAL_LIVES;
	int points = 0;
	float invulnerableTime = 0.0f;
};