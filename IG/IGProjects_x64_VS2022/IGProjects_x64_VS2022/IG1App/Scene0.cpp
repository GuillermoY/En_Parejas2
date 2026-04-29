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
	Sphere* sphereY = new Sphere(100.0, 100.0, 100.0);
	sphereY->setMaterial(Material(glm::vec3(1.0f, 0.84f, 0.0f),0));
	sphereY->setModelMat(translate(sphereY->modelMat(), glm::vec3(-150.0, 0, 0)));
	gObjects.push_back(sphereY); // Añadimos círculo amarillo estándar

	//Dorado
	Sphere* sphereG = new Sphere(100.0, 100.0, 100.0);
	sphereG->setMaterial(Material(glm::vec3(1.0f, 0.84f, 0.0f)));
	sphereG->setModelMat(translate(sphereG->modelMat(), glm::vec3(150.0, 0, 0)));
	gObjects.push_back(sphereG); // Añadimos círculo amarillo estándar

}