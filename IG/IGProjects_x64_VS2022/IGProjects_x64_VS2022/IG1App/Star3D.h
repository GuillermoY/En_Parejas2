#ifndef STAR3D_H
#define STAR3D_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"

class Star3D : public EntityWithTexture
{
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h, Texture* tex);
	void render(const glm::mat4& modelViewMat) const override; // Ap 27
	void update() override; // Ap 28

	float selfRotation = 0.0f;
};

#endif // STAR3D_H