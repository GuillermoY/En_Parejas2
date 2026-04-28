#include "Scene8.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene8::init()
{

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	Sphere* planet = new Sphere(150, 150.0, 150.0);
		
	planet->setMaterial(Material(glm::vec3{0.67,0.13,0.28}));
	gObjects.push_back(planet);

	droid = new Droid(20);
	//gObjects.push_back(droid);

	// AP 70: Nodo Ficticio
	inventedNode = new CompoundEntity();
	inventedNode->addEntity(droid);
	gObjects.push_back(inventedNode);
	droid->setModelMat(translate(mat4(1.0f), glm::vec3(0.0f, 170, 0.0f)));

	//inventedNodeBall = new CompoundEntity();
	////inventedNodeBall->addEntity(droid->getFirst());
	//gObjects.push_back(inventedNodeBall);
	//droid->setModelMat(translate(mat4(1.0f), glm::vec3(0.0f, 170, 0.0f)));
	
	// AP 77:
	PosLight* posLight = new PosLight();
	posLight->setPosition(glm::vec3(0.0f, 170, 0.0f));
	//posLight->setDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
	posLight->setAmb(glm::vec3(0.25f, 0.25f, 0.25f));
	posLight->setDiff(glm::vec3(0.6f, 0.6f, 0.6f));
	posLight->setSpec(glm::vec3(0.0f, 0.2f, 0.0f));
	posLight->setEnabled(true);
	gLights.push_back(posLight);
}

void
Scene8::rotates()
{
	inventedNode->setModelMat(rotate(inventedNode->modelMat(), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void
Scene8::orbit()
{
	inventedNode->setModelMat(rotate(inventedNode->modelMat(), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	droid->rotateBall(20.0f);
}

void
Scene8::setGL()
{
	glClearColor(0., 0., 0., 1.); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
}