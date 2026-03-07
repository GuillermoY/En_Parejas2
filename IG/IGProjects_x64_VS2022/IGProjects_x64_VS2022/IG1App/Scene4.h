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

		gObjects.push_back(new Ground(550.0f, 550.0f, 4, 4, baldosaTex));

		Texture* outTex = new Texture();
		outTex->load("../assets/images/papelE.png");

		Texture* intTex = new Texture();
		intTex->load("../assets/images/container.jpg");

		Abs_Entity* box = new BoxOutline(50.0f, outTex, intTex);
		box->setModelMat(translate(box->modelMat(), glm::vec3(200.0f, 50.0f, -50.0f)));
		gObjects.push_back(box);
		 
		//gObjects.push_back(new Cube(200.0f));

		Texture* ruedTex = new Texture();
		ruedTex->load("../assets/images/rueda.png");

		Abs_Entity* star3D = new Star3D(40.0, 8, 40.0, ruedTex);
		star3D->setModelMat(translate(star3D->modelMat(), glm::vec3(200.0f, 50.0f, -50.0f)));
		gObjects.push_back(star3D);

		Texture* cristTex = new Texture();
		cristTex->load("../assets/images/windowC.png");

		gObjects.push_back(new GlassParapet(275.0,100.0, cristTex));

		Texture* photoTex = new Texture();
		Abs_Entity* photo = new Photo(150.0, 150.0, photoTex);
		photo->setModelMat(translate(photo->modelMat(), glm::vec3(0.0f, 0.0f, 5.0f)));
		gObjects.push_back(photo);
	}
protected:
};
#endif // RGBRECTANGLE_H