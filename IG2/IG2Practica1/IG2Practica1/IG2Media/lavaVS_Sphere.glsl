#version 330 core

layout(location = 0) in vec4 vertex;
layout(location = 2) in vec3 normal;

uniform mat4 modelViewProjMat;
uniform mat4 normalMatrix;

out vec2 vUv0;
out vec3 vNormal;

const float PI = 3.14159265;

void main()
{
    // UV esféricas generadas a partir de la posición (no usamos uv0)
    vec3 p = normalize(vertex.xyz);
    float u = atan(p.z, p.x) / (2.0 * PI) + 0.5;
    float v = asin(clamp(p.y, -1.0, 1.0)) / PI + 0.5;
    vUv0 = vec2(u * 2.0, v);   // el *2 repite la textura en horizontal para no verla estirada

    vNormal = normalize(mat3(normalMatrix) * normal);
    gl_Position = modelViewProjMat * vertex;
}