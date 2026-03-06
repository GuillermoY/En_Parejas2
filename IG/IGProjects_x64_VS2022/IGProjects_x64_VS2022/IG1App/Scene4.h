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

		//gObjects.push_back(new Ground(200.0f, 200.0f, 4, 4, baldosaTex));

		Texture* outTex = new Texture();
		outTex->load("../assets/images/papelE.png");

		Texture* intTex = new Texture();
		intTex->load("../assets/images/container.jpg");

		Abs_Entity* box = new BoxOutline(200.0f, outTex, intTex);
		//gObjects.push_back(box);
		// 
		//gObjects.push_back(new Cube(200.0f));

		Texture* ruedTex = new Texture();
		ruedTex->load("../assets/images/rueda.png");

		gObjects.push_back(new Star3D(100.0,8,100.0, ruedTex));

	}
protected:
};
#endif // RGBRECTANGLE_H