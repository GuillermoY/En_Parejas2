#ifndef SCENE0_H
#define SCENE0_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Entity.h"


class Scene0 : public Scene // En el apartado 6 he hecho una clase con el polígono y el círculo (con los ejes)
{
public:
	void init() override;
protected:
};
#endif // Scene 0 