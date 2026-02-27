#include "RGBTriangle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


RGBTriangle::RGBTriangle(GLdouble h) : EntityWithColors()
{
	mMesh = Mesh::generateRGBTriangle(h);
}



/// <summary>
/// Apartado 13:
/// Sumamos 1 a la rotaci?n del tri?ngulo en sentido horario, que se rotar? despu?s de 
/// trasladarse en la direcci?n antihoraria cuando va circulando sobre la circunferencia
/// </summary>
void RGBTriangle::update()
{
	selfRotation += 1;
	double y = 0 + orbitDiameter * sin(glm::radians(angle));
	double x = 0 + orbitDiameter * cos(glm::radians(angle));
	angle += 1.0f;
	mModelMat = glm::mat4(1.0);
	mModelMat = translate(mModelMat, glm::vec3(0, y, 0.0));
	mModelMat = translate(mModelMat, glm::vec3(x, 0, 0.0));
	mModelMat = rotate(mModelMat, glm::radians(selfRotation), glm::vec3(0, 0, -1.0f));
}
