#include "Bomb.h"

int Bomb::bombCount = 0;

Bomb::Bomb(SceneNode* node, SceneManager* sceneMng)
	: IG2Object(Vector3::ZERO, node, sceneMng)
{
	std::string id = std::to_string(bombCount++);

	//Cuerpo
	Entity* bodyEnt = mSM->createEntity(BODY_MESH);
	bodyEnt->setMaterialName("bomb/body");
	bodyNode = mNode->createChildSceneNode();
	bodyNode->attachObject(bodyEnt);
	sphereRadius = bodyEnt->getBoundingBox().getHalfSize().y;
	bodyNode->setPosition(0, MAX_RADIUS, 0);
	setBodyRadius(MIN_RADIUS);

	//Mecha
	Entity* fuseEnt = mSM->createEntity(FUSE_MESH);
	fuseEnt->setMaterialName("bomb/fuse");
	fuseNode = mNode->createChildSceneNode();
	fuseNode->attachObject(fuseEnt);

	Vector3 fuseMeshSize = fuseEnt->getBoundingBox().getSize();
	fuseNode->setScale(FUSE_WIDTH / fuseMeshSize.x,
		FUSE_LENGTH / fuseMeshSize.y,
		FUSE_WIDTH / fuseMeshSize.z);

	fuseBaseY = MAX_RADIUS + MIN_RADIUS;
	fuseTopY = fuseBaseY + FUSE_LENGTH;
	fuseNode->setPosition(0, fuseBaseY + FUSE_LENGTH / 2.0f, 0);

	//Fuego
	firePS = mSM->createParticleSystem("bombFire" + id, "bomb/fuseFire");
	fireNode = mNode->createChildSceneNode();
	fireNode->setPosition(0, fuseTopY, 0);
	fireNode->attachObject(firePS);
	firePS->setEmitting(false);

	//Humo
	smokePS = mSM->createParticleSystem("bombSmoke" + id, "bomb/explosionSmoke");
	smokeNode = mNode->createChildSceneNode();
	smokeNode->setPosition(0, MAX_RADIUS, 0);
	smokeNode->attachObject(smokePS);
	smokePS->setEmitting(false);

	setModelVisible(false);
}

void Bomb::initExplosion(Vector3 pos)
{
	setPosition(pos);

	state = ACTIVE;
	timer = 0.0f;
	growing = true;
	setBodyRadius(MIN_RADIUS);

	smokePS->setEmitting(false);
	smokePS->clear();

	fireNode->setPosition(0, fuseTopY, 0);
	firePS->clear();
	firePS->setEmitting(true);

	setModelVisible(true);
}

bool Bomb::update(Real dt)
{
	if (state == ACTIVE)
	{
		timer += dt;

		//Crecimiento y decrecimiento
		Real speed = (MAX_RADIUS - MIN_RADIUS) / GROW_TIME;
		Real radius = currentRadius + (growing ? speed : -speed) * dt;
		if (radius >= MAX_RADIUS) {
			radius = MAX_RADIUS;
			growing = false;
		}
		else if (radius <= MIN_RADIUS) {
			radius = MIN_RADIUS;
			growing = true;
		}
		setBodyRadius(radius);

		//Fuego bajando por la mecha
		Real progress = timer / FUSE_TIME;
		if (progress > 1.0f)
			progress = 1.0f;
		fireNode->setPosition(0, fuseTopY + (fuseBaseY - fuseTopY) * progress, 0);

		if (timer >= FUSE_TIME) {
			explode();
			return true;
		}
	}
	else if (state == EXPLODING)
	{
		timer += dt;
		if (timer >= SMOKE_TIME) {
			smokePS->setEmitting(false);
			state = INACTIVE;
		}
	}
	return false;
}

void Bomb::explode()
{
	state = EXPLODING;
	timer = 0.0f;

	firePS->setEmitting(false);
	firePS->clear();
	setModelVisible(false);

	smokePS->setEmitting(true);
}

void Bomb::setBodyRadius(Real radius)
{
	currentRadius = radius;
	Real s = radius / sphereRadius;
	bodyNode->setScale(s, s, s);
}

void Bomb::setModelVisible(bool visible)
{
	bodyNode->setVisible(visible);
	fuseNode->setVisible(visible);
	fireNode->setVisible(visible);
}