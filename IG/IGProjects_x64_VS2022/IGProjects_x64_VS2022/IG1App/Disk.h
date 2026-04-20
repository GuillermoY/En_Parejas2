#include "ColorMaterialEntity.h"
#include "IndexMesh.h"
#include <vector>
#include <glm/glm.hpp>

class Disk : public ColorMaterialEntity {
public:
    Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples);
};