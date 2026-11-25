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
    // Normalizamos la normal
    vec3 n = normalize(normal);

    // Calculamos el desplazamiento
    float offset = sin(vertex.x * frequency + time)
                 * cos(vertex.z * frequency + time)
                 * amplitude;

    // Desplazamos el vértice
    vec3 newPosition = vertex.xyz + n * offset;

    // Pasamos las coordenadas de textura
    vUv0 = uv0;

    // Calculamos la posición final
    gl_Position = modelViewProjMat * vec4(newPosition, 1.0);
}