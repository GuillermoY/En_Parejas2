#version 330 core

in vec2 vUv0;

uniform sampler2D lavaTexture;

out vec4 fFragColor;

void main()
{
    fFragColor = texture(lavaTexture, vUv0);
}