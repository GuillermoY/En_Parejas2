#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();
	virtual void update(); // Apartado 12.2: Método virtual update que llama al update de cada objeto.

	void render(Camera const& cam) const;

	// load or unload scene data into the GPU
	void load();
	void unload();

protected:
	void destroy();
	void setGL();
	void resetGL();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
};

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
		gObjects.push_back(new RegularPolygon(360, 200.0, 0, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))); // Añadimos circunferencia amarilla (red, green, blue, alpha), lados 360
		gObjects.push_back(new RegularPolygon(6, 200.0, 0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))); // Añadimos hexágono magenta (red, green, blue, alpha), lados 6
	}
protected:
};

/// <summary>
/// Apartado 10:
/// </summary>
class Scene2 : public Scene
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

		gObjects.push_back(new RGBTriangle(100, 200)); // Triángulo como el apartado 7 y 11 (Con pos en (R,0))
		gObjects.push_back(new RGBRectangle(280, 280)); // Cuadrado como el apartado 8
		gObjects.push_back(new RegularPolygon(360, 200.0, 0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f))); // Circunferencia como el apartado 5

	}
protected:
};
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

	}
protected:
};

#endif //_H_Scene_H_
