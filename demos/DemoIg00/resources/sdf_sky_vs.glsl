#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aLocal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec3 aParams; // radiusX, radiusY, edgeWidth

uniform mat4 uViewProjectionMatrix;

out vec2 vLocal;
out vec4 vColor;
out vec3 vParams;

void main()
{
	vLocal = aLocal;
	vColor = aColor;
	vParams = aParams;
	gl_Position = uViewProjectionMatrix * vec4(aPosition, 0.0, 1.0);
}
