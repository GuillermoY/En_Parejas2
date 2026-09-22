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
#include "Block.h"

using namespace Ogre;

class Empty : public Block {
public:
	Empty(Vector3 initPos, SceneNode* node, SceneManager* sceneMng)
		: Block(initPos, node, sceneMng) { };
};
