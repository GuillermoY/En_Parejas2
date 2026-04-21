#include "Torus.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>

using namespace glm;
/// <summary>
/// AP 56
/// </summary>
/// <param name="R"> radio rosquilla </param>
/// <param name="r"> Grosor rosquilla </param>
/// <param name="nPoints"> número de muestras </param>
/// <param name="nSamples"> número de puntos con que se aproxima a la circunferencia </param>
Torus::Torus(GLdouble R, GLdouble r, GLuint nPoints, GLuint nSamples)
{
    std::vector<vec2> profile;

    for (GLuint i = 0; i <= nPoints; ++i) {
        float theta = 2.0f * std::numbers::pi * i / nPoints;

        float x = R + r * cos(theta); // desplazamiento
        float y = r * sin(theta);

        profile.emplace_back(x, y);
    }

    mMesh = IndexMesh::generateByRevolution(profile, nSamples, 2.0f * pi<float>());
}