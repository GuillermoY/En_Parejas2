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
#include "IG2Object.h"

using namespace Ogre;

class Block : public IG2Object {
public:
	Block(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: IG2Object(initPos, node, sceneMng, mesh) , mMesh(mesh) { };
	Block(Vector3 initPos, SceneNode* node, SceneManager* sceneMng)
		: IG2Object(initPos, node, sceneMng) { };

	bool canPassThrough()
	{
		return mMesh != "";
	}
	String mMesh="";
};