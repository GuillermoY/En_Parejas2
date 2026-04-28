#include "Scene9.h"
#include "Entity.h"
#include "Sphere.h"
#include "SnowMan.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene9::init()
{
	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	gObjects.push_back(new SnowMan(150.0f));
}
