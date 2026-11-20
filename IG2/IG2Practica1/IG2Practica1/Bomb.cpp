#include "Bomb.h"

int Bomb::bombCount = 0;

Bomb::Bomb(SceneNode* node, SceneManager* sceneMng)
	: IG2Object(Vector3::ZERO, node, sceneMng)
{
	std::string id = std::to_string(bombCount++);

	//------------------------------------------------------------------------
	// Cuerpo: esfera. Su centro queda a MAX_RADIUS del suelo
	Entity* bodyEnt = mSM->createEntity(BODY_MESH);
	bodyEnt->setMaterialName("bomb/body");
	bodyNode = mNode->createChildSceneNode();
	bodyNode->attachObject(bodyEnt);
	sphereRadius = bodyEnt->getBoundingBox().getHalfSize().y;
	bodyNode->setPosition(0, MAX_RADIUS, 0);
	setBodyRadius(MIN_RADIUS);

	//------------------------------------------------------------------------
	// Mecha: sale de la parte de arriba del cuerpo (en su tamano minimo) y NO se escala con el
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

	//------------------------------------------------------------------------
	// Fuego de la mecha (duracion indefinida: se controla con setEmitting)
	firePS = mSM->createParticleSystem("bombFire" + id, "bomb/fuseFire");
	fireNode = mNode->createChildSceneNode();
	fireNode->setPosition(0, fuseTopY, 0);
	fireNode->attachObject(firePS);
	firePS->setEmitting(false);

	//------------------------------------------------------------------------
	// Humo de la explosion: uno por casilla alcanzable (duracion indefinida,
	// se para con un temporizador). Se colocan al explotar, en showSmoke
	for (int i = 0; i < MAX_SMOKE_CELLS; i++) {
		ParticleSystem* ps = mSM->createParticleSystem(
			"bombSmoke" + id + "_" + std::to_string(i), "bomb/explosionSmoke");
		SceneNode* n = mNode->createChildSceneNode();
		n->attachObject(ps);
		ps->setEmitting(false);

		smokePS.push_back(ps);
		smokeNodes.push_back(n);
	}

	// Al crearse, la bomba esta guardada (invisible)
	setModelVisible(false);
}

void Bomb::initExplosion(Vector3 pos)
{
	setPosition(pos);

	state = ACTIVE;
	timer = 0.0f;
	growing = true;
	setBodyRadius(MIN_RADIUS);

	// Por si se reutiliza mientras aun quedaba humo de la explosion anterior
	stopSmoke(true);

	// El fuego empieza en el extremo de la mecha
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

		// Animacion 1: el cuerpo crece y decrece
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

		// Animacion 2: el fuego baja por la mecha hasta el cuerpo
		Real progress = timer / FUSE_TIME;
		if (progress > 1.0f)
			progress = 1.0f;
		fireNode->setPosition(0, fuseTopY + (fuseBaseY - fuseTopY) * progress, 0);

		// Se acabo la mecha
		if (timer >= FUSE_TIME) {
			explode();
			return true;
		}
	}
	else if (state == EXPLODING)
	{
		timer += dt;
		if (timer >= SMOKE_TIME) {
			// Deja de emitir; las particulas que quedan se desvanecen solas
			stopSmoke(false);
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
	// El humo lo enciende el laberinto con showSmoke, que es quien sabe
	// hasta donde llega la explosion
}

void Bomb::showSmoke(const std::vector<Vector3>& cells)
{
	Vector3 bombPos = getPosition();

	for (size_t i = 0; i < cells.size() && i < smokePS.size(); i++) {
		// Los nodos de humo son hijos de la bomba: posicion relativa a ella
		Vector3 local = cells[i] - bombPos;
		smokeNodes[i]->setPosition(local.x, MAX_RADIUS, local.z);
		smokePS[i]->clear();
		smokePS[i]->setEmitting(true);
	}
}

void Bomb::stopSmoke(bool clear)
{
	for (ParticleSystem* ps : smokePS) {
		ps->setEmitting(false);
		if (clear)
			ps->clear();
	}
}

void Bomb::setBodyRadius(Real radius)
{
	currentRadius = radius;
	Real s = radius / sphereRadius;
	bodyNode->setScale(s, s, s);   // solo el cuerpo: la mecha no se escala
}

void Bomb::setModelVisible(bool visible)
{
	// No se usa IG2Object::setVisible porque ocultaria tambien el humo
	bodyNode->setVisible(visible);
	fuseNode->setVisible(visible);
	fireNode->setVisible(visible);
}