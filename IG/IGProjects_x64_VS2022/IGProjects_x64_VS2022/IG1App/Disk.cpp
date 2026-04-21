#include "Disk.h"

Disk::Disk(GLdouble R, GLdouble r, GLuint nRings, GLuint nSamples)
    : ColorMaterialEntity(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f))
{
    std::vector<glm::vec2> profile;
    for (GLuint i = 0; i < nRings; ++i) {
        GLdouble x = r + (R - r) * i / (nRings - 1);
        profile.emplace_back(x, 0.0);
    }
    mMesh = IndexMesh::generateByRevolution(profile, nSamples);
}