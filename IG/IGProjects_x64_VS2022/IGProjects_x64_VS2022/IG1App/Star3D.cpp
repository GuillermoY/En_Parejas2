#include "Star3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


Star3D::Star3D(GLdouble re, GLuint np, GLdouble h, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateStar3DTexCor(re,np,h);
}

void Star3D::render(const glm::mat4& modelViewMat) const // Ap: 27
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate); // Cargamos textura en la GPU 

		if (mTexture != nullptr)
			mTexture->bind();

		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();

		mat4 rot = rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 bMat = modelViewMat * mModelMat * rot;
		upload(bMat);

		if (mTexture != nullptr)
			mTexture->bind();

		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();
	}
}

void Star3D::update() // Ap: 28
{
	selfRotation += 1;
	mModelMat = glm::mat4(1.0);
	mModelMat = rotate(mModelMat, glm::radians(selfRotation), glm::vec3(0, -1.0f, 0.0f)); // Girar sobre su eje Y
	mModelMat = rotate(mModelMat, glm::radians(selfRotation), glm::vec3(0, 0, -1.0f)); // Rotan sobre su eje Z
}