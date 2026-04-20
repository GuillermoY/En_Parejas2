#include "ColorMaterialEntity.h"
#include "IndexMesh.h"
#include <vector>
#include <glm/glm.hpp>

class Cone : public ColorMaterialEntity {
public:
    Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples);
};