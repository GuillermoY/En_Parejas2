#include "Photo.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


Photo::Photo(GLdouble length, GLdouble height, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateRectangleTexCor(length, height,1,1);

	mModelMat = glm::mat4(1.0);
	mModelMat = rotate(mModelMat, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
}

void 
Photo::update() // Ap: 35
{
	mTexture->loadColorBuffer(800, 600,GL_BACK);
}