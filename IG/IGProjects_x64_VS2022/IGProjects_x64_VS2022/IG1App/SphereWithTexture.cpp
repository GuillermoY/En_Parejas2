#include "SphereWithTexture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


SphereWithTexture::SphereWithTexture(GLdouble radius, GLuint nParallels, GLuint nMeridians, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = IndexMesh::generateSphere(radius,nParallels,nMeridians);
}