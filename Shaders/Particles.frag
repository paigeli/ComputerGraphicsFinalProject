#version 330 core
// in vec2 TexCoords;
// in vec4 ParticleColor;
in vec3 fcolor;
out vec4 color;

// uniform sampler2D sprite;

void main()
{
  // color = (texture(sprite, TexCoords) * ParticleColor);
  //color = vec4(1.0,1.0,1.0,1.0);
  color = min((vec4(fcolor, 1.0)), vec4(1,1,1,1));
}  