#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in float aShapeIndex;

out float vShapeIndex;

uniform mat4 uViewProjectionMatrix;

void main()
{
   gl_Position = uViewProjectionMatrix * vec4(aPosition, 0.0, 1.0);
   vShapeIndex = aShapeIndex;
}
