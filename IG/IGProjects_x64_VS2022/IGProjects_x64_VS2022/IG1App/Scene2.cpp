#include "Scene2.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene2::init()
{
	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	RGBTriangle* triangle = new RGBTriangle(100);
	triangle->setModelMat(translate(triangle->modelMat(), glm::vec3(200, 0, 0.0)));
	gObjects.push_back(triangle); // Triángulo como el apartado 7 y 11 (Con pos en (R,0))
	gObjects.push_back(new RGBRectangle(280, 280)); // Cuadrado como el apartado 8
	gObjects.push_back(new RegularPolygon(360, 200.0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))); // Circunferencia como el apartado 5


}