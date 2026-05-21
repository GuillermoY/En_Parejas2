#include "EntityWithMaterial.h"

bool EntityWithMaterial::mShowNormals = false;

using namespace glm;
EntityWithMaterial::EntityWithMaterial() {
	mShader = Shader::get("light");
}

void
EntityWithMaterial::render(const mat4& modelViewMat) const
{
	mShader->use();
	// Carga los atributos del material en la GPU
	mat4 aMat = modelViewMat * mModelMat;
	mMaterial.upload(*mShader);
	upload(aMat);
	mMesh->render();
    if (mShowNormals) {
        Shader* normShader = Shader::get("normals");
        normShader->use();
        normShader->setUniform("modelView", aMat);
        mMesh->render();
    }
}