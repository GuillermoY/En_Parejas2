#ifndef GLASSPARAPET_H
#define GLASSPARAPET_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"

class GlassParapet : public EntityWithTexture
{
public:
	explicit GlassParapet(GLdouble length, GLdouble height, Texture* tex);
};

#endif // GLASSPARAPET_H