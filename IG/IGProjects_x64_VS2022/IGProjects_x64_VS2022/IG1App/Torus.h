#ifndef TORUS_H
#define TORUS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "IndexMesh.h"
#include "ColorMaterialEntity.h"


class Torus : public ColorMaterialEntity

{
public:
	explicit Torus(GLdouble R, GLdouble r, GLuint nPoints = 40, GLuint nSamples = 40);
};
#endif // TORUS_H