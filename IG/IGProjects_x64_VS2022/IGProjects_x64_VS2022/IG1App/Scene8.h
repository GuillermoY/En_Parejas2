#ifndef SCENE8_H
#define SCENE8_H
#include "Scene.h"
#include "CompoundEntity.h"
#include "Entity.h"
#include "Sphere.h"
#include "Droid.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

/// <summary>
/// AP 69: 
/// </summary>
class Scene8 : public Scene
{
protected:
    CompoundEntity* mInventedNode;
    Droid* mDroid;
    PosLight* mPosLight;
    SpotLight* mSpotLight;
public:
    void init() override;

    void rotates() override;
    void orbit() override;
    void alterPosLight() { mPosLight->setEnabled(!mPosLight->enabled()); };
    void alterSpotLight() { mSpotLight->setEnabled(!mSpotLight->enabled()); };
    void alterDroidLight() { if (mDroid) mDroid->alterLight(); };

    void setGL() override;
    void handleKeys(unsigned int key) override;
};
#endif