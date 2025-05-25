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

// Radius of trajectory of points
const float p0traj_radius = 0.2;
const float p1traj_radius = 0.24;
const float ptraj_speed0 = 0.1;
const float ptraj_speed1 = 0.54;
const vec2 ptraj_center = vec2(0.5, 0.5);

void main()
{
   if (uTime < 0.0) discard; // ensure uniform will not be optimized away

   float angle0 = uTime * ptraj_speed0;
   float angle1 = uTime * ptraj_speed1;
   vec2 p0 = vec2(
      cos(angle0) * p0traj_radius + ptraj_center.x,
      sin(angle0) * p0traj_radius + ptraj_center.y
   );
   vec2 p1 = vec2(
      cos(angle1 + M_PI) * p1traj_radius + ptraj_center.x,
      sin(angle1 + M_PI) * p1traj_radius + ptraj_center.y
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
