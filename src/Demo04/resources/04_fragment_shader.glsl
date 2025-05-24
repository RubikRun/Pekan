#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform float uMixFactor;

void main()
{
   vec4 tex1 = texture(uTex0, vTexCoord);
   vec4 tex2 = texture(uTex1, vTexCoord);
   FragColor = mix(tex1, tex2, uMixFactor);
}
