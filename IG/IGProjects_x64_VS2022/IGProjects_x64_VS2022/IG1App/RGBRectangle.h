#ifndef RGBRECTANGLE_H
#define RGBRECTANGLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"


class RGBRectangle : public EntityWithColors
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	//static Mesh* generateRectangle(GLdouble w, GLdouble h);
};
#endif // RGBRECTANGLE_H