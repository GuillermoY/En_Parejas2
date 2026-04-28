#include "EntityWithMaterial.h"

using namespace glm;
EntityWithMaterial::EntityWithMaterial() {
	mShader = Shader::get("light");
}

void
EntityWithMaterial::render(const mat4& modelViewMat) const
{
	mShader->use();
	// Carga los atributos del material en la GPU
	mMaterial.upload(*mShader);
	upload(modelViewMat * mModelMat);
	mMesh->render();
}