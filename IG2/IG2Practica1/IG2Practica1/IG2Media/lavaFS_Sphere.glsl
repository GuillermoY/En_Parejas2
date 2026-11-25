#version 330 core

in vec2 vUv0;
in vec3 vNormal;

uniform sampler2D lavaTexture;
uniform sampler2D blackTexture;
uniform float Flipping;
uniform vec4 light_direction_;

out vec4 fFragColor;

void main()
{
    bool frontFacing = (Flipping > -1.0) ? gl_FrontFacing : !gl_FrontFacing;

    vec3 lavaColor  = texture(lavaTexture,  vUv0).rgb;
    vec3 blackColor = texture(blackTexture, vUv0).rgb;

    // Huecos
    if (lavaColor.r > 0.6)
        discard;

    vec3 color = frontFacing ? lavaColor : blackColor;

    vec3 N = normalize(vNormal);
    if (!frontFacing) N = -N;

    vec3 L = normalize(-light_direction_.xyz);
    float diff = max(dot(N, L), 0.0);

    fFragColor = vec4(color * diff, 1.0);
}