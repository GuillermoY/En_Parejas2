#include "GlassParapet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


GlassParapet::GlassParapet(GLdouble length, GLdouble height, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mModelMat = scale(mModelMat, glm::vec3(1.0f,0.2f,1.0f));
}

void GlassParapet::render(const glm::mat4& modelViewMat) const // Ap: 27
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		mShader->setUniform("modulate", mModulate); // Cargamos textura en la GPU 
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mShader->use();
		upload(aMat);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		glBlendColor(1.0f, 1.0f, 1.0f, 0.7f);

		mTexture->bind();
		mMesh->render();
		mTexture->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}