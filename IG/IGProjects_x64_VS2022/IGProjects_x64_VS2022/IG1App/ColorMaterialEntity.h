#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// AP 58: Entidad con color y material que usa el shader simple_light.
/// AP 63: Renderiza normales con shader "normals" si mShowNormals es true.
/// </summary>
class ColorMaterialEntity : public SingleColorEntity
{
private:
    static bool mShowNormals; // AP 63: controla si se muestran las normales

public:
    ColorMaterialEntity(glm::vec4 color = glm::vec4(1.0f));

    void render(const glm::mat4& modelViewMat) const override; // AP 63

    // AP 63: activa/desactiva la visualización de normales (tecla N)
    static void toggleShowNormals() { mShowNormals = !mShowNormals; }
};