#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;

uniform vec2 uPosition;

void main()
{
	gl_Position = vec4(aPos.x + uPosition.x, aPos.y + uPosition.y, 0.0, 1.0);
	vTexCoord = aTexCoord;
}
