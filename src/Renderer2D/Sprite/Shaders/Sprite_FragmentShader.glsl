#version 330 core

in vec2 vTexCoord;
in float vTexIndex;
out vec4 FragColor;

// TODO: at runtime, replace this value with the actual maximum for current hardware
#define MAX_TEXTURES 32

uniform sampler2D uTextures[MAX_TEXTURES];

void main()
{
    int index = int(vTexIndex + 0.5);
    FragColor = texture(uTextures[index], vTexCoord);
}
