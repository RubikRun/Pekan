#version 330 core

in vec2 vTexCoord;
in vec4 vTint;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
	vec4 tex = texture(uTexture, vTexCoord);
	// Soften the color filter so cells stay brighter (pure multiply was too dark)
	vec3 filterColor = mix(vec3(1.0), vTint.rgb, 0.45);
	filterColor = min(filterColor * 1.25 + 0.08, vec3(1.0));
	FragColor = vec4(tex.rgb * filterColor, tex.a * vTint.a);
	if (FragColor.a < 0.01)
	{
		discard;
	}
}
