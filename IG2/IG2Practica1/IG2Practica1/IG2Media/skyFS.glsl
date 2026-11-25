#version 330 core

in vec2 vUvZoom;
in vec2 vUvOriginal;

uniform sampler2D lightMap;
uniform sampler2D skyTexture;

uniform float lightTime;

out vec4 fFragColor;

void main()
{
    vec4 maskColor = texture(lightMap, vUvOriginal);
    vec4 skyColor = texture(skyTexture, vUvZoom);

    float lightFactor = 0.75 + 0.25 * lightTime;

    fFragColor = vec4(
        skyColor.rgb * maskColor.rgb * lightFactor,
        skyColor.a
    );
}