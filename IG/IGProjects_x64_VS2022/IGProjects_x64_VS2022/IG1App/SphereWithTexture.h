#ifndef SPHEREWITHTEXTURE_H
#define SPHEREWITHTEXTURE_H
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "IndexMesh.h"
#include "Entity.h"

class SphereWithTexture : public EntityWithTexture
{
public:
	explicit SphereWithTexture(GLdouble radius, GLuint nParallels, GLuint nMeridians, Texture* tex);
};
#endif