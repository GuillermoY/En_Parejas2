#ifndef SPHERE_H
#define SPHERE_H

#include "ColorMaterialEntity.h"
#include "IndexMesh.h"
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Sphere : public ColorMaterialEntity {
public:
    Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians);
};

#endif