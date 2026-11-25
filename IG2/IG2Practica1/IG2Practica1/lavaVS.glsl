#version 330 core

in vec4 vertex;
in vec3 normal;
in vec2 uv0;

uniform mat4 modelViewProjMat;
uniform float time;
uniform float amplitude;
uniform float frequency;

out vec2 vUv0;

void main()
{
    vec3 n = normalize(normal);

    float offset = sin(vertex.x * frequency + time)
                 * cos(vertex.z * frequency + time)
                 * amplitude;

    vec3 newPosition = vertex.xyz + n * offset;

    vUv0 = uv0;

    gl_Position = modelViewProjMat * vec4(newPosition, 1.0);
}