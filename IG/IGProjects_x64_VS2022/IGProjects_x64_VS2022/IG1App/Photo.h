#ifndef PHOTO_H
#define PHOTO_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Entity.h"
#include "Texture.h"
class IG1App;
class Photo : public EntityWithTexture
{
public:
	explicit Photo(GLdouble length, GLdouble height, Texture* tex);
	void update() override; // Ap 35	
};

#endif // PHOTO_H