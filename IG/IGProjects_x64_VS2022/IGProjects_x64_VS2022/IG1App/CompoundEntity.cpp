#include "CompoundEntity.h"

using namespace glm;

CompoundEntity::~CompoundEntity()
{// release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();

	for (Texture* el : gTextures)
		delete el;

	gTextures.clear();

	for (Light* el : gLights)
		delete el;

	gLights.clear();
}

void
CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
}

void
CompoundEntity::addTexture(Texture* tex)
{
	gTextures.push_back(tex);
}

// AP 79
void
CompoundEntity::addLight(Light* el)
{
	gLights.push_back(el);
}

void CompoundEntity::update()
{
	for (Abs_Entity* obj : gObjects) {
		obj->update();
	}
}

void
CompoundEntity::render(const glm::mat4& modelViewMat) const
{
	mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
	// Renderizar entidades hijas
	for (Abs_Entity* el : gObjects)
		el->render(aMat);

	uploadLights(aMat);
}

void
CompoundEntity::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();
}

void
CompoundEntity::unload()
{
	for (Abs_Entity* obj : gObjects)
	{
		obj->unload();
		//delete obj;
	}

	for (Light* obj : gLights)
	{
		obj->unload(*Shader::get("simple_light"));
	}
}

// AP 79
void
CompoundEntity::uploadLights(glm::mat4& aMat) const
{
	for (Light* el : gLights)
	{
		el->upload(*Shader::get("light"), aMat);
	}
}
