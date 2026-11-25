#version 330 core

in vec2 vUv0;
in vec2 vUv1;

uniform sampler2D lavaTexture;
uniform sampler2D blackTexture;
vec3 light_direction_; 
uniform float Flipping;

out vec4 fFragColor;

void main()
{
    fFragColor = texture(lavaTexture, vUv0);
    fFragColor = texture(blackTexture, vUv1);
}