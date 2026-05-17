#include "Scene0.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.h"
#include "Light.h"
#include "SphereWithTexture.h"

using namespace glm;

void
Scene0::init()	
{
	// allocate memory and load resources
	// Lights
	// Textures
	// Graphics objects (entities) of the scene
	//DirLight* sun = new DirLight();
	//sun->setDirection(glm::vec3(-1, -1, -1));
	//sun->setEnabled(true);
	//gLights.push_back(sun);

	//Apartado 73
	//Amarillo
	Sphere* sphereY = new Sphere(100.0, 100.0, 40.0);
	sphereY->setMaterial(Material(glm::vec3(1.0f, 1.0f, 0.0f),5));
	sphereY->setModelMat(translate(sphereY->modelMat(), glm::vec3(200.0, 0.0f, 0.0)));
	gObjects.push_back(sphereY); // Añadimos círculo amarillo estándar

	//Dorado
	Sphere* sphereG = new Sphere(100.0, 100.0, 40.0);
	Material gold;
	gold.setAmb(glm::vec4(.25f, .2f, .7f, 1.0f));
	gold.setDiff(glm::vec4(.75f, .6f, .2f, 1.0f));
	gold.setSpec(glm::vec4(.63f,.56f,.37f,1.0f));
	gold.setExp(51.2f);
	sphereG->setMaterial(gold);
	sphereG->setModelMat(translate(sphereG->modelMat(), glm::vec3(0.0, 0, 200.0)));
	gObjects.push_back(sphereG); // Añadimos círculo amarillo estándar

}