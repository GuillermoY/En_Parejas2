#ifndef SCENE4_H
#define SCENE4_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Texture.h"

class Scene4 : public Scene
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

		Texture* baldosaTex = new Texture();
		baldosaTex->load("../assets/images/baldosaC.png");

		gObjects.push_back(new Ground(200.0f, 200.0f, baldosaTex));
		//gObjects.push_back(new Cube(200.0f));

	}
protected:
};
#endif // RGBRECTANGLE_H