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
	Villain(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh, bool isOG = false)
		: Character(initPos, node, sceneMng, mesh), isOG(isOG) {
	};

	int getSpeed() override { return VILLAIN_SPEED; }
	bool getOG() { return isOG; };
	virtual void update(Ogre::Real time) {};
private:
	const int VILLAIN_SPEED = 100;
	bool isOG = false;

};