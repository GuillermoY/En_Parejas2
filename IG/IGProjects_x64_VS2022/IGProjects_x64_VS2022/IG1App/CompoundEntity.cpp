#include "CompoundEntity.h"

using namespace glm;

CompoundEntity::~CompoundEntity()
{// release memory and resources

	for (Abs_Entity* el : gObjects)
		delete el;

	gObjects.clear();
}

void
CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
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
}