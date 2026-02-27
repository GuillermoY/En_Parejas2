#ifndef RGBTRIANGLE_H
#define RGBTRIANGLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"


class RGBTriangle : public EntityWithColors
{
public:
	explicit RGBTriangle(GLdouble h);
	void update() override;
	double angle = 0.0f;
	float selfRotation = 0.0f;
	float orbitDiameter = 200.0f;
	//static Mesh* generateRectangle(GLdouble w, GLdouble h);
};

#endif // RGBRECTANGLE_H