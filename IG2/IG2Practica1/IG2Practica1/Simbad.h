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
	};

protected:

};