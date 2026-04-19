#include "ColorMaterialEntity.h"

using namespace glm;

// AP 63: definición del estático
bool ColorMaterialEntity::mShowNormals = false;

ColorMaterialEntity::ColorMaterialEntity(glm::vec4 color)
    : SingleColorEntity(color)
{
    // AP 58: usamos el shader simple_light en lugar de simple
    mShader = Shader::get("simple_light");
}

// AP 63
void ColorMaterialEntity::render(const mat4& modelViewMat) const
{
    if (mMesh != nullptr) {
        mat4 aMat = modelViewMat * mModelMat;
        mShader->use();
        upload(aMat);
        mShader->setUniform("color", mColor);
        mMesh->render();

        // Segunda pasada con shader de normales (si está activado)
        if (mShowNormals) {
            Shader* normShader = Shader::get("normals");
            normShader->use();
            normShader->setUniform("modelView", aMat);
            mMesh->render();
        }
    }
}