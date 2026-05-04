#include "ColorMaterialEntity.h"

using namespace glm;

// AP 63: definición del estático
bool ColorMaterialEntity::mShowNormals = false;

ColorMaterialEntity::ColorMaterialEntity(glm::vec3 color)
    : EntityWithMaterial()
{
    // AP 73:
    setMaterial(Material(color));
    mShader = Shader::get("light");
    // AP 58: usamos el shader simple_light en lugar de simple
    //mShader = Shader::get("simple_light");
}

// AP 63
void ColorMaterialEntity::render(const mat4& modelViewMat) const
{
    if (mMesh != nullptr) {
        mat4 aMat = modelViewMat * mModelMat;
        mShader->use();
        // Carga los atributos del material en la GPU
        upload(aMat);
        mMaterial.upload(*mShader);
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