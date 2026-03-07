	#include "Ground.h"
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>

	using namespace glm;


	Ground::Ground(GLdouble w, GLdouble h, GLuint rw, GLuint rh, Texture* tex) : EntityWithTexture(tex)
	{
		mMesh = Mesh::generateRectangleTexCor(w, h, rw, rh);
		mModelMat = glm::mat4(1.0);
		mModelMat = rotate(mModelMat, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)); 
	}