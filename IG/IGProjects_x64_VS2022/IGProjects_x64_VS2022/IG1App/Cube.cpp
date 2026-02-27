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
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW); // El sentido de la cara frontal es Counter ClockWise (Antihorario)

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();


		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}