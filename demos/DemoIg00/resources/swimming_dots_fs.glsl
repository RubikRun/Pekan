#version 330 core

in vec2 vCorner;
in vec4 vColor;

out vec4 FragColor;

void main()
{
	if (dot(vCorner, vCorner) > 1.0)
	{
		discard;
	}
	FragColor = vColor;
}
