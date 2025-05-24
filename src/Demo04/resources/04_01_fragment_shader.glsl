#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform float uMixFactor;


void main()
{
   if (uMixFactor < 0.0) discard; // ensure uniform will not be optimized away

   vec2 p0 = vec2(0.25, 0.25);
   vec2 p1 = vec2(0.75, 0.75);

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
