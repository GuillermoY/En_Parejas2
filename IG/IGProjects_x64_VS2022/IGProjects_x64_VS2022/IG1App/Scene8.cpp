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

	mDroid = new Droid(20);
	//gObjects.push_back(droid);
	Sphere* planet = new Sphere(150, 150.0, 150.0);

	planet->setMaterial(Material(glm::vec3{ 0.67,0.13,0.28 }));
	gObjects.push_back(planet);
	// AP 70: Nodo Ficticio
	mInventedNode = new CompoundEntity();
	mInventedNode->addEntity(mDroid);
	gObjects.push_back(mInventedNode);
	mDroid->setModelMat(translate(mat4(1.0f), glm::vec3(0.0f, 170, 0.0f)));

	//inventedNodeBall = new CompoundEntity();
	////inventedNodeBall->addEntity(droid->getFirst());
	//gObjects.push_back(inventedNodeBall);
	//droid->setModelMat(translate(mat4(1.0f), glm::vec3(0.0f, 170, 0.0f)));
	
	// AP 77:
	mPosLight = new PosLight();
	mPosLight->setPosition(glm::vec3(0.0f, 170, 0.0f));
	mPosLight->setAmb(glm::vec3(0.25f, 0.25f, 0.25f));
	mPosLight->setDiff(glm::vec3(0.6f, 0.6f, 0.6f));
	mPosLight->setSpec(glm::vec3(0.0f, 0.2f, 0.0f));
	mPosLight->setEnabled(true);
	gLights.push_back(mPosLight);

	// AP 78:
	mSpotLight = new SpotLight(glm::vec3{ 0, 0, 200 }, 0);
	mSpotLight->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
	mSpotLight->setAmb(glm::vec3(0.25f, 0.25f, 0.25f));
	mSpotLight->setDiff(glm::vec3(0.6f, 0.6f, 0.6f));
	mSpotLight->setSpec(glm::vec3(0.0f, 1.0f, 0.0f));
	mSpotLight->setEnabled(true);
	mSpotLight->setCutoff(12.0f, 17.0f);
	gLights.push_back(mSpotLight);
	//gLights.push_back(mDroid->getLight()); 
}

void
Scene8::rotates()
{
	mInventedNode->setModelMat(rotate(mInventedNode->modelMat(), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void
Scene8::orbit()
{
	mInventedNode->setModelMat(rotate(mInventedNode->modelMat(), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	mDroid->rotateBall(20.0f);
}

void
Scene8::setGL()
{
	glClearColor(0., 0., 0., 1.); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
}