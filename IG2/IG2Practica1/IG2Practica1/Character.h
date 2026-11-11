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
		: IG2Object(initPos, node, sceneMng, mesh), sM(sceneMng) {
	};
	typedef enum { UP, DOWN, LEFT, RIGHT } tDir;

	static Vector3 dirToVector(tDir dir);
	static tDir opposite(tDir dir);
	virtual int getSpeed();

	bool isDirectionModified();
	Vector3 getNexDirVector();
	Quaternion getQuaternionForNewDirection();

	void changeDirection(tDir dir);
	void rotateToNewDirection();
	bool is180Turn();
	tDir getDirectiorion() { return sinbadDirectorion; };
	virtual void update(Real dt) { if (charactAnim != nullptr) charactAnim->addTime(dt); };

	void setAnimState(String anim)
	{
		if (charactAnim->getAnimationName() == anim && charactAnim != nullptr)
			return;

		if (charactAnim != nullptr) 
			charactAnim->setEnabled(false);

		charactAnim = entity->getAnimationState(anim);
		charactAnim->setTimePosition(0);
		charactAnim->setLoop(true);
		charactAnim->setEnabled(true);
	}
protected:
	SceneManager* sM;

	Ogre::AnimationState* charactAnim = nullptr;
private:
	tDir sinbadDirectorion = DOWN;
	static const int SPEED; 
};