#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"

class Cube : public SingleColorEntity
{
public:
	explicit Cube(GLdouble l);
	void render(const glm::mat4& modelViewMat) const override;
};
#endif // CUBE_H