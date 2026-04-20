#include "ColorMaterialEntity.h"
#include "IndexMesh.h"
#include <vector>
#include <glm/glm.hpp>

class Sphere : public ColorMaterialEntity {
public:
    Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians);
};