#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform float uTime;

#define M_PI 3.1415926535897932384626433832795

const float pradius_inner = 0.006;
const float pradius_outer = 0.008;
const vec4 pcolor_inner = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 pcolor_outer = vec4(1.0, 1.0, 1.0, 1.0);

const float p0traj_radius = 0.2;
const float p0traj_speed = 0.1;
const vec2 p0traj_center = vec2(0.5, 0.5);

const float p1traj_ampl = 0.2;
const float p1traj_scale = 12.0;
const float p1traj_speed = 0.05;
const vec2 p1traj_center = vec2(0.5, 0.5);

float osc(float x)
{
	int xi = int(x);
	if (xi % 2 == 0)
	{
		return x - float(xi);
	}
	else
	{
		return 1.0 - (x - float(xi));
	}
}

void main()
{
	if (uTime < 0.0) discard; // ensure uniform will not be optimized away

	float angle0 = uTime * p0traj_speed;
	vec2 p0 = vec2(
		cos(angle0) * p0traj_radius + p0traj_center.x,
		sin(angle0) * p0traj_radius + p0traj_center.y
	);

	float p1x = osc(uTime * p1traj_speed) - 0.5 + p1traj_center.x;
	vec2 p1 = vec2(
		p1x,
		sin(p1x * p1traj_scale) * p1traj_ampl + p1traj_center.y
	);

	if (distance(p0, vTexCoord) < pradius_inner || distance(p1, vTexCoord) < pradius_inner)
	{
		FragColor = pcolor_inner;
		return;
	}
	else if (distance(p0, vTexCoord) < pradius_outer || distance(p1, vTexCoord) < pradius_outer)
	{
		FragColor = pcolor_outer;
		return;
	}

	vec2 p0p1 = vec2(p1.x - p0.x, p1.y - p0.y);
	vec2 p0v = vec2(vTexCoord.x - p0.x, vTexCoord.y - p0.y);

	// Calculate distance from p0 to vTexCoord across the direction p0->p1
	float p0dist = dot(p0v, normalize(p0p1));

	float blend = p0dist / distance(p0, p1);
	blend = clamp(blend, 0.0, 1.0);

	vec4 tex0 = texture(uTex0, vTexCoord);
	vec4 tex1 = texture(uTex1, vTexCoord);

	FragColor = mix(tex0, tex1, blend);
}
