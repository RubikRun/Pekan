#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
   FragColor = texture(tex0, vTexCoord);
}
