#include "Photo.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "IG1App.h"

using namespace glm;


Photo::Photo(GLdouble length, GLdouble height, Texture* tex) : EntityWithTexture(tex)
{
	mMesh = Mesh::generateRectangleTexCor(length, height,1,1);
}

void 
Photo::update() // Ap: 35
{
	Viewport view = IG1App::s_ig1app.viewPort();
	mTexture->loadColorBuffer(view.width(), view.height(), GL_BACK);
}