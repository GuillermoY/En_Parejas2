#include "Scene5.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Torus.h"
using namespace glm;

void
Scene5::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0));

	Torus* torus = new Torus(200, 100);
	gObjects.push_back(torus);
	torus->setColor({ 0.13,0.54,0.13,1 });
}