#pragma once

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <string>
#include "IG2Object.h"

using namespace Ogre;

class Bomb : public IG2Object {
public:
	Bomb(SceneNode* node, SceneManager* sceneMng);

	void initExplosion(Vector3 pos);
	bool update(Real dt);

private:
	enum State { INACTIVE, ACTIVE, EXPLODING };

	const String BODY_MESH = "sphere.mesh";
	const String FUSE_MESH = "cube.mesh";

	static constexpr Real FUSE_TIME = 3.0f;
	static constexpr Real GROW_TIME = 0.5f;
	static constexpr Real SMOKE_TIME = 1.0f;

	static constexpr Real MIN_RADIUS = 35.0f;
	static constexpr Real MAX_RADIUS = 50.0f;
	static constexpr Real FUSE_LENGTH = 60.0f;
	static constexpr Real FUSE_WIDTH = 6.0f;

	State state = INACTIVE;
	Real timer = 0.0f;
	bool growing = true;
	Real currentRadius = MIN_RADIUS;

	SceneNode* bodyNode = nullptr;
	SceneNode* fuseNode = nullptr;
	SceneNode* fireNode = nullptr;
	SceneNode* smokeNode = nullptr;

	ParticleSystem* firePS = nullptr;
	ParticleSystem* smokePS = nullptr;

	Real sphereRadius = 1.0f;
	Real fuseBaseY = 0.0f;
	Real fuseTopY = 0.0f;

	void setBodyRadius(Real radius);
	void setModelVisible(bool visible);
	void explode();

	static int bombCount;       //para que los sistemas de partículas tengan nombres únicos
};