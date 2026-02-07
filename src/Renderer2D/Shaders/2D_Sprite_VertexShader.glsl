#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 uViewProjectionMatrix;

void main()
{
	gl_Position = uViewProjectionMatrix * vec4(aPosition, 0.0, 1.0);
	vTexCoord = aTexCoord;
}
