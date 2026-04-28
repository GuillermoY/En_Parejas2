#ifndef DROID_H
#define DROID_H

#include "CompoundEntity.h"
#include "IndexMesh.h"
#include "Cone.h"
#include "SphereWithTexture.h"
#include "Disk.h"
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Droid : public CompoundEntity {
private:
    Texture* ballTexture;
    SphereWithTexture* ballBody;
public:
    Droid(GLdouble radius);
    void rotateBall(float degrees);
    ~Droid();
};

#endif