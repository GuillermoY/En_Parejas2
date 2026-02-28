#ifndef BOXOUTLINE_H
#define BOXOUTLINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"

class BoxOutline : public EntityWithTexture
{
protected:
	Texture* mInteriorTexture;
public:
	explicit BoxOutline(GLdouble length, Texture* tex, Texture* intTex);
	void render(const glm::mat4& modelViewMat) const override;
};

#endif // BOXOUTLINE_H