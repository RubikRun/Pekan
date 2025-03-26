#version 330 core
layout (location = 0) in ivec2 aPos;

uniform vec2 uResolution;

void main()
{
   gl_Position = vec4(aPos.x * 2.0 / uResolution.x - 1.0, 1.0 - aPos.y * 2.0 / uResolution.y, 0.0, 1.0);
}
