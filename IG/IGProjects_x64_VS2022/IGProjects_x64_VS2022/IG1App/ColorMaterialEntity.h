#ifndef COLORMATERIALENTITY_H
#define COLORMATERIALENTITY_H

//#include "Entity.h"
#include "EntityWithMaterial.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// AP 58: Entidad con color y material que usa el shader simple_light.
/// AP 63: Renderiza normales con shader "normals" si mShowNormals es true.
/// AP 73: ColorMaterialEntity sea una subclase trivial de EntityWithMaterial 
/// que tenga como material uno con todas sus componentes a un color fijo.

/// </summary>
class ColorMaterialEntity : public EntityWithMaterial
{
private:
    static bool mShowNormals; // AP 63: controla si se muestran las normales

public:
    ColorMaterialEntity(glm::vec3 color = glm::vec4(1.0f));

    void render(const glm::mat4& modelViewMat) const override; // AP 63

    // AP 63: activa/desactiva la visualización de normales (tecla N)
    static void toggleShowNormals() { mShowNormals = !mShowNormals; }
};
#endif