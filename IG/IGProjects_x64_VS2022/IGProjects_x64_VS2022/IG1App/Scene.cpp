#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Scene::Scene()
{
	init();
}

void
Scene::init() // En el apartado 6 he hecho una clase vacía con solo los ejes
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new RGBAxes(400.0));
	dirLight = new DirLight();
	dirLight->setDirection(normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
	dirLight->setAmb(glm::vec3(0.25f, 0.25f, 0.25f));
	dirLight->setDiff(glm::vec3(0.6f, 0.6f, 0.6f));
	dirLight->setSpec(glm::vec3(0.0f, 0.2f, 0.0f));
	dirLight->setEnabled(true);
	gLights.push_back(dirLight);
}

void Scene::update()
{
	for (Abs_Entity* obj : gObjects) {
		obj->update();
	}

	for (Abs_Entity* obj : gTrsObjects) {
		obj->update();
	}
}

Scene::~Scene()
{
	destroy();
	resetGL();
}

void
Scene::destroy()
{ // release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();

	for (Abs_Entity* el : gTrsObjects)
		delete el;

	gTrsObjects.clear();


	for (Texture* el : gTextures)
		delete el;

	gTextures.clear();


	for (Light* el : gLights)
		delete el;

	gLights.clear();
}

void
Scene::load()
{
	setGL();
	for (Abs_Entity* obj : gObjects)
		obj->load();

	for (Abs_Entity* obj : gTrsObjects)
		obj->load();
}

void
Scene::unload()
{
	for (Abs_Entity* obj : gObjects)
		obj->unload();

	for (Abs_Entity* obj : gTrsObjects)
		obj->unload();

	for (Light* obj : gLights)
	{
		Shader* shader = Shader::get("light");
		shader->use();
		obj->unload(*shader);
	}

	resetGL();
}

void
Scene::setGL()
{
	// OpenGL basic setting
	// Cambiamos fondo de la escena para el apartado 1, 
	// red, green, blue, alpha)
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	uploadLights(cam);

	for (Abs_Entity* el : gObjects)
		el->render(cam.viewMat());


	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Abs_Entity* el : gTrsObjects)
		el->render(cam.viewMat());

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void
Scene::uploadLights(Camera const& cam) const
{
	Shader* shader = Shader::get("light");
	shader->use();
	for (Light* el : gLights)
	{
		el->upload(*shader, cam.viewMat());
	}
	//glm::vec4 lightDirWorld(-1.0f, -1.5f, -1.25f, 0.0f);
	//glm::vec4 lightDirView = normalize(cam.viewMat() * lightDirWorld);
	//shader->setUniform("lightDir", lightDirView);
}

void 
Scene::handleKeys(unsigned int key)
{
	switch (key) {
	case 'r':
		toggleLight();
		break;
	default:
		break;
	}
}