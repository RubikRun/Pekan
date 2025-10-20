#version 330 core
out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

// Offsets for sampling neighbors
const float offset_x = 1.0 / 573.0;
const float offset_y = 1.0 / 326.0;
vec2 offsets[9] = vec2[](
    vec2(-offset_x,     offset_y), // top-left
    vec2( 0.0f,         offset_y), // top-center
    vec2( offset_x,     offset_y), // top-right
    vec2(-offset_x,     0.0f),   // center-left
    vec2( 0.0f,         0.0f),   // center
    vec2( offset_x,     0.0f),   // center-right
    vec2(-offset_x,    -offset_y), // bottom-left
    vec2( 0.0f,        -offset_y), // bottom-center
    vec2( offset_x,    -offset_y)  // bottom-right
);

uniform float kernel[9];

void main()
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
