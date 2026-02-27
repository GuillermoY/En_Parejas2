#ifndef GROUND_H
#define GROUND_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"

class Ground : public EntityWithTexture
{
public:
	explicit Ground(GLdouble h, GLdouble l, Texture* tex) ;
	virtual void render(const glm::mat4& modelViewMat) const override;
};

#endif // RGBRECTANGLE_H