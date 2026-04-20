#include "IndexedBox.h"

IndexedBox::IndexedBox(GLdouble l)
    : ColorMaterialEntity(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)) // color verde
{
    // AP 64: usamos la versión de 24 vértices para normales correctas
    mMesh = IndexMesh::generateIndexedBox(l);
}