#include "Scene4.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene4::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	Texture* baldosaTex = new Texture();
	baldosaTex->load("../assets/images/baldosaC.png");
	gTextures.push_back(baldosaTex);

	gObjects.push_back(new Ground(550.0f, 550.0f, 4, 4, baldosaTex));

	Texture* outTex = new Texture();
	outTex->load("../assets/images/papelE.png");
	gTextures.push_back(outTex);

	Texture* intTex = new Texture();
	intTex->load("../assets/images/container.jpg");
	gTextures.push_back(intTex);

	Abs_Entity* box = new BoxOutline(50.0f, outTex, intTex);
	box->setModelMat(translate(box->modelMat(), glm::vec3(150.0f, 50.0f, -130.0f)));
	gObjects.push_back(box);

	//gObjects.push_back(new Cube(200.0f));

	Texture* ruedTex = new Texture();
	ruedTex->load("../assets/images/rueda.png");
	gTextures.push_back(ruedTex);

	glm::vec3 starPos = glm::vec3(150.0f, 50.0f, -130.0f);
	Abs_Entity* star3D = new Star3D(30.0, 8, 30.0, ruedTex, starPos);
	star3D->setModelMat(translate(star3D->modelMat(), starPos));
	gObjects.push_back(star3D);

	Texture* cristTex = new Texture();
	cristTex->load("../assets/images/windowC.png", 160);
	gTextures.push_back(cristTex);

	gTrsObjects.push_back(new GlassParapet(275.0, 100.0, cristTex));

	Texture* photoTex = new Texture();
	gTextures.push_back(photoTex);
	Abs_Entity* photo = new Photo(150.0, 150.0, photoTex);
	photo->setModelMat(translate(photo->modelMat(), glm::vec3(0.0f, 5.0f, 5.0f)));
	photo->setModelMat(rotate(photo->modelMat(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	photo->setModelMat(rotate(photo->modelMat(), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	gObjects.push_back(photo);
}