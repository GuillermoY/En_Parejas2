#version 330 core

in vec4 vertex;
in vec2 uv0;

uniform mat4 modelViewProjMat;
uniform float zoomTime;

out vec2 vUvZoom;
out vec2 vUvOriginal;

void main()
{
    float zoomFactor = 0.75 + 0.25 * zoomTime;

    vUvZoom = (uv0 - vec2(0.5)) * zoomFactor + vec2(0.5);

    vUvOriginal = uv0;

    gl_Position = modelViewProjMat * vertex;
}