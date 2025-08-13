#version 330 core

in vec2 vTexCoord;
in float vTexIndex;
in vec4 vColor;
out vec4 FragColor;

#define MAX_TEXTURES 32

uniform sampler2D uTextures[MAX_TEXTURES];

void main()
{
    // Sample sprite's color from sprite's 2D texture
    vec4 spriteColor = texture(uTextures[int(vTexIndex + 0.5)], vTexCoord);

    // Check if we are currently rendering a sprite or a shape
    float isSprite = float(vTexIndex >= 0.0);

    // Output either shape's color or sprite's color
    // depending on whether we are rendering a shape or a sprite
    FragColor = mix(vColor, spriteColor, isSprite);
}
