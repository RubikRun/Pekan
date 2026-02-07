#version 330 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
	vec4 spriteColor = texture(uTexture, vTexCoord);
	FragColor = spriteColor;
}
