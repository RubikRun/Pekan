#version 330 core
layout(location = 0) in vec2 a_position;

uniform mat4 u_viewProjectionMatrix;

void main()
{
   gl_Position = u_viewProjectionMatrix * vec4(a_position, 0.0, 1.0);
}
