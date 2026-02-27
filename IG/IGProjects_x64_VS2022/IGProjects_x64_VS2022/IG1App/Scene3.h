#ifndef SCENE3_H
#define SCENE3_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Scene.h"


class Scene3 : public Scene
{
public:
	void init() override
	{
		setGL(); // OpenGL settings

		// allocate memory and load resources
		// Lights
		// Textures

		// Graphics objects (entities) of the scene
		gObjects.push_back(new RGBAxes(400.0));

		gObjects.push_back(new RGBCube(200.0f));
		//gObjects.push_back(new Cube(200.0f));

	}
protected:
};
#endif // RGBRECTANGLE_H