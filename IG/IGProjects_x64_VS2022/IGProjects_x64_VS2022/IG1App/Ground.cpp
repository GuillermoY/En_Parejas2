#include "Ground.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


Ground::Ground(GLdouble h, GLdouble l, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateRGBRectangle(h, l);
}

void
Ground::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr && mTexture != nullptr) {
		mTexture->bind();
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
	}
}
