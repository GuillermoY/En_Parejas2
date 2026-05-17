#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

/// <summary>
/// Apartado 9:
/// Redefinimos el render para RGBRectangle, que, usando culling, hacemos que la trasera
/// está rellena y la delantera use líneas
/// </summary>
Cube::Cube(GLdouble l)
{
	mMesh = Mesh::generateCube(l);
}
void Cube::render(const glm::mat4& modelViewMat) const
{
	lineRender(modelViewMat);
}