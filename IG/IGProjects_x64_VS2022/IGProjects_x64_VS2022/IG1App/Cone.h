#ifndef CONE_H
#define CONE_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "IndexMesh.h"
#include "ColorMaterialEntity.h"

class Cone : public ColorMaterialEntity {
public:
    explicit Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples);
};

#endif