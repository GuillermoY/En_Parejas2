#include "BoxOutline.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


BoxOutline::BoxOutline(GLdouble length, Texture* tex, Texture* intTex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mInteriorTexture = intTex;
}
/*
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mShader->setUniform("modulate", mModulate); // Cargamos textura en la GPU

		if (mTexture != nullptr)
			mTexture->bind();

		mMesh->render();

		if (mTexture != nullptr)
			mTexture->unbind();
	}
*/
void BoxOutline::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW); // El sentido de la cara frontal es Counter ClockWise (Antihorario)

		mTexture->bind();
		glCullFace(GL_FRONT);
		mMesh->render();
		mTexture->unbind();


		mInteriorTexture->bind();
		glCullFace(GL_BACK);
		mMesh->render();
		mInteriorTexture->unbind();

		// Habilitamos las dos caras, cada una con su relleno particular
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
}