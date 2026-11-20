#pragma once

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>
#include <string>
#include <vector>
#include "IG2Object.h"

using namespace Ogre;

// Bomba: cuerpo (esfera) + mecha (cilindro) + fuego de la mecha + humo de la explosion.
// mNode (el nodo del IG2Object) no se escala nunca: es el "contenedor" de todas las piezas.
class Bomb : public IG2Object {
public:
	Bomb(SceneNode* node, SceneManager* sceneMng);

	// Coloca la bomba en pos (a ras de suelo) y enciende la mecha
	void initExplosion(Vector3 pos);

	// Avanza las animaciones. Devuelve true SOLO en el frame en que explota
	bool update(Real dt);

	// Muestra humo en cada una de las casillas afectadas (posiciones del mundo)
	void showSmoke(const std::vector<Vector3>& cells);

	// Casillas que alcanza la explosion en cada direccion
	static constexpr int EXPLOSION_RANGE = 5;

private:
	enum State { INACTIVE, ACTIVE, EXPLODING };

	// Mallas
	const String BODY_MESH = "sphere.mesh";
	const String FUSE_MESH = "cube.mesh";   // cambialo por un cilindro si tienes uno en IG2Media

	// Tiempos (segundos)
	static constexpr Real FUSE_TIME = 3.0f;    // desde que se pone hasta que explota
	static constexpr Real GROW_TIME = 0.5f;    // lo que tarda en crecer (y en decrecer)
	static constexpr Real SMOKE_TIME = 1.0f;   // tiempo que se emite humo tras explotar

	// Tamanos (unidades del mundo; un bloque mide 200)
	static constexpr Real MIN_RADIUS = 35.0f;
	static constexpr Real MAX_RADIUS = 50.0f;
	static constexpr Real FUSE_LENGTH = 60.0f;
	static constexpr Real FUSE_WIDTH = 6.0f;

	State state = INACTIVE;
	Real timer = 0.0f;          // tiempo que lleva activa (o explotando)
	bool growing = true;        // creciendo / decreciendo
	Real currentRadius = MIN_RADIUS;

	SceneNode* bodyNode = nullptr;
	SceneNode* fuseNode = nullptr;
	SceneNode* fireNode = nullptr;
	ParticleSystem* firePS = nullptr;

	// Un sistema de humo por casilla que puede alcanzar la explosion:
	// la del centro + EXPLOSION_RANGE en cada una de las 4 direcciones
	static constexpr int MAX_SMOKE_CELLS = 1 + 4 * EXPLOSION_RANGE;
	std::vector<SceneNode*> smokeNodes;
	std::vector<ParticleSystem*> smokePS;
	void stopSmoke(bool clear);

	Real sphereRadius = 1.0f;   // radio de sphere.mesh sin escalar
	Real fuseBaseY = 0.0f;      // altura (local) donde la mecha toca el cuerpo
	Real fuseTopY = 0.0f;       // altura (local) del extremo de la mecha

	void setBodyRadius(Real radius);
	void setModelVisible(bool visible);
	void explode();

	static int bombCount;       // para que los sistemas de particulas tengan nombres unicos
};