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
	typedef enum { UP, DOWN, LEFT, RIGHT } tDir;

	static Vector3 dirToVector(tDir dir);
	static tDir opposite(tDir dir);
	virtual int getSpeed();

	tDir sinbadDirectorion = DOWN;
	bool isDirectionModified();
	Vector3 getNexDirVector();
	Quaternion getQuaternionForNewDirection();

	void changeDirection(tDir dir);
	void rotateToNewDirection();
	bool is180Turn();
private:
	static const int SPEED; 
};