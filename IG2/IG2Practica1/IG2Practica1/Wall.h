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

class Wall : public Block {
public:
	Wall(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: Block(initPos, node, sceneMng, mesh) { };
};