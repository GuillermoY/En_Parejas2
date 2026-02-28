#ifndef GROUND_H
#define GROUND_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble w, GLdouble h, GLuint rw, GLuint rh, Texture* tex);
};

#endif // GROUND_H