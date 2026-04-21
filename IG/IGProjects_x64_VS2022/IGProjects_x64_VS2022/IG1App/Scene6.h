#ifndef SCENE6_H
#define SCENE6_H
#include "Scene.h"
#include "Entity.h"
//#include "ColorMaterialEntity.h"

/// <summary>
/// AP 62/64: Escena con un IndexedBox iluminado.
/// Se activa con la tecla 6.
/// </summary>
class Scene6 : public Scene
{
public:
    void init() override;
};
#endif