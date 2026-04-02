#include "GlassParapet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


GlassParapet::GlassParapet(GLdouble length, GLdouble height, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	mModelMat = scale(mModelMat, glm::vec3(1.0f,0.2f,1.0f));
}