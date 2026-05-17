#include "CompoundEntity.h"

using namespace glm;

CompoundEntity::~CompoundEntity()
{// release memory and resources

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
CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
}

void
CompoundEntity::addTrsEntity(Abs_Entity* ae)
{
	gTrsObjects.push_back(ae);
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
	for (Abs_Entity* obj : gTrsObjects) {
		obj->update();
	}
}

void
CompoundEntity::render(const glm::mat4& modelViewMat) const
{
	mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
	// Renderizar entidades hijas
	uploadLights(aMat);
	
	for (Abs_Entity* el : gObjects)
		el->render(aMat);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (Abs_Entity* el : gTrsObjects)
		el->render(aMat);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void
CompoundEntity::load()
{
	for (Abs_Entity* obj : gObjects)
		obj->load();

	for (Abs_Entity* obj : gTrsObjects)
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

	for (Abs_Entity* obj : gTrsObjects)
	{
		obj->unload();
		//delete obj;
	}

	Shader* shader = Shader::get("light");
	shader->use();
	for (Light* obj : gLights)
	{
		obj->unload(*shader);
	}
}

// AP 79
void
CompoundEntity::uploadLights(glm::mat4& aMat) const
{
	Shader* shader = Shader::get("light");
	shader->use();
	//shader->use();
	for (Light* el : gLights)
	{
		el->upload(*shader, aMat);
	}
}
