#include "Cone.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Cone::Cone(GLdouble h, GLdouble r, GLdouble R, GLuint nRings, GLuint nSamples)
    : ColorMaterialEntity(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f))
{
    std::vector<glm::vec2> profile;
    for (GLuint i = 0; i < nRings; ++i) {
        GLdouble t = (nRings > 1) ? (GLdouble)i / (nRings - 1) : 0.0;
        profile.emplace_back(r + (R - r) * t, h * t);
    }
    mMesh = IndexMesh::generateByRevolution(profile, nSamples);
}