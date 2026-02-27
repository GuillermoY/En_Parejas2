#ifndef RGBCUBE_H
#define RGBCUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"


class RGBCube : public EntityWithColors

{
public:
	explicit RGBCube(GLdouble l);
};
#endif // RGBCUBE_H