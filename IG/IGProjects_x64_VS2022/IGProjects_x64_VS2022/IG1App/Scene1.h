#ifndef SCENE1_H
#define SCENE1_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Entity.h"


class Scene1 : public Scene // En el apartado 6 he hecho una clase con el polígono y el círculo (con los ejes)
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

		//Apartado 5
		gObjects.push_back(new RegularPolygon(360, 200.0, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))); // Añadimos circunferencia amarilla (red, green, blue, alpha), lados 360
		gObjects.push_back(new RegularPolygon(6, 200.0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))); // Añadimos hexágono magenta (red, green, blue, alpha), lados 6
	}
protected:
};
#endif // RGBRECTANGLE_H