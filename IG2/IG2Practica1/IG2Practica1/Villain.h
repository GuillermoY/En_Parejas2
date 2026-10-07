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

class Villain: public Character {
public:
	Villain(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: Character(initPos, node, sceneMng, mesh) {
	};

	int getSpeed() override { return VILLAIN_SPEED; }

private:
	const int VILLAIN_SPEED = 100;

};