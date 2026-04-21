#ifndef SCENE7_H
#define SCENE7_H
#include "Scene.h"
#include "Entity.h"
//#include "ColorMaterialEntity.h"

/// <summary>
/// AP 68: Escena con ...
/// Se activa con la tecla 7.
/// </summary>
class Scene7 : public Scene
{
public:
    void init() override;
};
#endif