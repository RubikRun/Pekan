#version 330 core

layout (location = 0) in vec2 aCorner;
layout (location = 1) in vec2 aCenter;
layout (location = 2) in float aRadius;
layout (location = 3) in vec4 aColor;

uniform mat4 uViewProjectionMatrix;

out vec2 vCorner;
out vec4 vColor;

void main()
{
	vCorner = aCorner;
	vColor = aColor;
	vec2 worldPos = aCenter + aCorner * aRadius;
	gl_Position = uViewProjectionMatrix * vec4(worldPos, 0.0, 1.0);
}
