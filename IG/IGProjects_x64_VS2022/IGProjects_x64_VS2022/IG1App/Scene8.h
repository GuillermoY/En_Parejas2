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
    CompoundEntity* inventedNode;
    Droid* droid;
public:
    void init() override;

    void rotates() override;
    void orbit() override;

    void setGL() override;
};
#endif