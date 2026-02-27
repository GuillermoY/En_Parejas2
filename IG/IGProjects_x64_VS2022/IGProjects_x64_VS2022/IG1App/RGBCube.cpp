#include "RGBCube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

RGBCube::RGBCube(GLdouble l)
{
	mMesh = Mesh::generateRGBCubeTriangles(l);
}