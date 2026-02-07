#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform float uTime;

void main()
{
	vec4 tex0 = texture(uTex0, vTexCoord);
	vec4 tex1 = texture(uTex1, vTexCoord);
	FragColor = mix(tex0, tex1, cos(uTime / 5.0) * 0.5 + 0.5);
}
