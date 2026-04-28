#include "Scene5.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Torus.h"
//#include "Sphere.h"
//#include "Cone.h"
//#include "Disk.h"
using namespace glm;

void
Scene5::init()
{
	Torus* torus = new Torus(200, 100);
	gObjects.push_back(torus);
	torus->setMaterial(Material(glm::vec4{ 0,1,0,1 }));

	//gObjects.push_back(new Sphere(150.0, 150.0,150.0));
	//gObjects.push_back(new Cone(150.0, 0.0, 150.0,150.0,150.0));
	//gObjects.push_back(new Disk(150.0,10,150.0,150.0));
}