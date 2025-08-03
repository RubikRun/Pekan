#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexIndex;

uniform mat4 uViewProjectionMatrix;

out vec2 vTexCoord;
out float vTexIndex;

void main()
{
    gl_Position = uViewProjectionMatrix * vec4(aPosition, 0.0, 1.0);
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
}
