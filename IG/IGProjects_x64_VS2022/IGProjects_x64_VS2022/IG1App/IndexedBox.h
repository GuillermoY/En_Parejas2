#ifndef INDEXEDBOX_H
#define INDEXEDBOX_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "IndexMesh.h"
#include "ColorMaterialEntity.h"

/// <summary>
/// AP 62: Cubo indexado que hereda de ColorMaterialEntity para tener iluminación.
/// AP 64: Usa generateIndexedBox (24 vértices) en lugar de generateIndexedBox8 (8 vértices)
///        para tener normales correctas por cara.
/// </summary>
class IndexedBox : public ColorMaterialEntity
{
public:
    explicit IndexedBox(GLdouble l);
};
#endif