#ifndef _H_MaterialEntity_H_
#define _H_MaterialEntity_H_

#include "Entity.h"
#include "Material.h"
#include "Light.h"
#include <glm/glm.hpp>

/// <summary>
/// AP: 71 para renderizar las entidades con 
/// material iluminadas usando el shader light
/// </summary>
class EntityWithMaterial : public Abs_Entity
{
public:
	static bool mShowNormals;
	EntityWithMaterial();
	void setMaterial(const Material& m) { mMaterial = m; };
	void render(const glm::mat4& modelViewMat) const override;

	// AP 63: activa/desactiva la visualización de normales (tecla N)
	static void toggleShowNormals() { mShowNormals = !mShowNormals; }
protected:
	Material mMaterial;
};
#endif //_H_Material_H_

