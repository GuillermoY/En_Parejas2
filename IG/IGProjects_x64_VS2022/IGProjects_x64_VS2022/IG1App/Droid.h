#ifndef DROID_H
#define DROID_H

#include "CompoundEntity.h"
#include "IndexMesh.h"
#include "Cone.h"
#include "SphereWithTexture.h"
#include "Disk.h"
#include "Light.h"
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Droid : public CompoundEntity {
private:
    Texture* ballTexture;
    SphereWithTexture* ballBody;
    SpotLight* mDroidLight;
public:
    Droid(GLdouble radius);
    void rotateBall(float degrees);
    void alterLight() { mDroidLight->setEnabled(!mDroidLight->enabled()); };
    SpotLight* getLight() { return mDroidLight; };
};

#endif