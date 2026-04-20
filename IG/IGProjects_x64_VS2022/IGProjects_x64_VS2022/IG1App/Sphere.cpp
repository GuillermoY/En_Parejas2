#include "Sphere.h"
#include <glm/gtc/constants.hpp>

Sphere::Sphere(GLdouble radius, GLuint nParallels, GLuint nMeridians)
    : ColorMaterialEntity(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f))
{
    std::vector<glm::vec2> profile;
    for (GLuint i = 0; i <= nParallels; ++i) {
        GLdouble theta = glm::radians(-90.0 + 180.0 * i / nParallels);
        profile.emplace_back(radius * cos(theta), radius * sin(theta));
    }
    mMesh = IndexMesh::generateByRevolution(profile, nMeridians);
    mMesh->load();
}