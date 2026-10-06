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

class InvisibleBlock : public Block {
public:
	InvisibleBlock(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: Block(initPos, node, sceneMng, mesh) {
		walkable = false;
		this->setVisible(visible);
	};

	virtual void update() override
	{
		if (currTime >= timeForChange)
		{
			this->setVisible(visible);
			visible = !visible;
			currTime = 0;
		}
		else
			currTime++;
	}
private:
	bool visible = false;
	int currTime = 0;
	int timeForChange = 50;
};
