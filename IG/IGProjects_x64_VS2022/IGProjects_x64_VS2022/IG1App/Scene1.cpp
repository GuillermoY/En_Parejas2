#include "Scene1.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene1::init()
{
	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	//Apartado 5
	gObjects.push_back(new RegularPolygon(360, 200.0, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))); // Añadimos circunferencia amarilla (red, green, blue, alpha), lados 360
	gObjects.push_back(new RegularPolygon(6, 200.0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))); // Añadimos hexágono magenta (red, green, blue, alpha), lados 6
}