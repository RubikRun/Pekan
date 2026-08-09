#version 330 core

in vec2 vLocal;
in vec4 vColor;
in vec3 vParams;

out vec4 FragColor;

void main()
{
	float radiusX = max(vParams.x, 0.0001);
	float radiusY = max(vParams.y, 0.0001);
	float edge = max(vParams.z, 0.0001);

	// Elliptical SDF in local space
	vec2 q = vec2(vLocal.x / radiusX, vLocal.y / radiusY);
	float r = length(q);
	float angle = atan(q.y, q.x);

	// Funky border: soft wavy SDF perturbation
	float warp = 0.07 * sin(5.0 * angle + 2.2 * r) + 0.035 * sin(9.0 * angle - 3.1 * r);
	float d = r - 1.0 + warp;

	float alpha = 1.0 - smoothstep(-edge, edge, d);
	if (alpha < 0.01)
	{
		discard;
	}

	FragColor = vec4(vColor.rgb, vColor.a * alpha);
}
