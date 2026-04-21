#ifndef SNOWMAN_H
#define SNOWMAN_H

#include "CompoundEntity.h"
#include "IndexMesh.h"
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class SnowMan : public CompoundEntity {
private:
    Texture* ballTexture;
public:
    SnowMan(GLdouble radius);
};

#endif