#ifndef COMPOUNDENTITY_H
#define COMPOUNDENTITY_H

#include "Entity.h"
#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// AP 66: Define la destructora ~CompoundEntity() y, sobre todo, reescribe convenientemente los
/// métodos render, update, load y unload.
/// </summary>
class CompoundEntity : public Abs_Entity
{
private:
	std::vector<Abs_Entity*> gObjects;
	std::vector<Abs_Entity*> gTrsObjects;
	std::vector<Texture*> gTextures;
	std::vector<Light*> gLights;
public:
	explicit CompoundEntity() {};
	~CompoundEntity();

	void addEntity(Abs_Entity* ae);
	void addTrsEntity(Abs_Entity* ae);
	void addTexture(Texture* mat);
	void addLight(Light* el);

	void update() override;

	void render(const glm::mat4& modelViewMat) const override;

	void load() override;
	void unload() override;

	// AP 79
	void uploadLights(glm::mat4& aMat) const;

	//Abs_Entity* getFirst() {
	//	if (gObjects[0]) 
	//	{ 
	//		return gObjects[0]; 
	//	}
	//	else
	//	{
	//		return nullptr;
	//	}
	//};
};
#endif