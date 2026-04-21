#ifndef DROID_H
#define DROID_H

#include "CompoundEntity.h"
#include "IndexMesh.h"
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Droid : public CompoundEntity {
private:
    Texture* ballTexture;
public:
    Droid(GLdouble radius);
    ~Droid();
};

#endif