#version 330 core

in float vShapeIndex;
out vec4 FragColor;

uniform sampler1D uShapesColorsTexture;
uniform int uShapesCount;

void main()
{
   FragColor = texture(uShapesColorsTexture, vShapeIndex / float(uShapesCount - 1));
}
