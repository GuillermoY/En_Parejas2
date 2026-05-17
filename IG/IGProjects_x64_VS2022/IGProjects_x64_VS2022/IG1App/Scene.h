#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Cube.h"
#include "RGBCube.h"
#include "RGBRectangle.h"
#include "RGBTriangle.h"
#include "Ground.h"
#include "BoxOutline.h"
#include "Star3D.h"
#include "GlassParapet.h"
#include "Photo.h"
#include "Light.h"

#include <vector>

class Scene
{
public:
	Scene();
	virtual ~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();
	virtual void update(); // Apartado 12.2: Método virtual update que llama al update de cada objeto.

	void render(Camera const& cam) const;

	// load or unload scene data into the GPU
	void load();
	void unload();

	// AP 70:
	virtual void rotates() {};
	virtual void orbit() {};

	// AP 73:
	void uploadLights(Camera const& cam) const;
	void toggleLight() { if (dirLight) dirLight->setEnabled(!dirLight->enabled()); }// AP 76: controla si se muestra la luz 

	virtual void handleKeys(unsigned int key);

protected:
	void destroy();
	// AP: 69
	virtual void setGL();
	void resetGL();


	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTrsObjects; // Entities (graphic translucent objects) of the scene

	std::vector<Texture*> gTextures; // Entities (textures) of the scene

	std::vector<Light*> gLights; // Entities (lights) of the scene
	DirLight* dirLight;
};


#endif //_H_Scene_H_
