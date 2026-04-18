#include "ColorMaterialEntity.h"

using namespace glm;
ColorMaterialEntity::ColorMaterialEntity()
{
    //mShader = Shader::get("simple_light");
}

//void
//ColorMaterialEntity::render(mat4 const& modelViewMat) const
//{
//    if (mMesh != nullptr) {
//        glm::mat4 aMat = modelViewMat * mModelMat;
//        mShader->use();
//        upload(aMat);
//        mShader->setUniform("simple_light", mColor);
//        mMesh->render();
//    }
//}