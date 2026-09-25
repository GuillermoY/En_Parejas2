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

class Character : public IG2Object {
public:
	Character(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, String mesh)
		: IG2Object(initPos, node, sceneMng, mesh) {
	};
private:
	static const int SPEED; 
protected:
	typedef enum { UP, DOWN, LEFT, RIGHT } tDir;
	tDir sinbadDirectorion = UP;
public:
	bool isDirectionModified();
	Vector3 getNexDirVector();
	Quaternion getQuaternionForNewDirection();
	int getSpeed();
	void rotateToNewDirection();
	bool is180Turn() {};
};