#version 330 core
layout (location = 0) in vec3 pos; // <vec2 position, vec2 texCoords>

// out vec2 TexCoords;
// out vec4 ParticleColor;
out vec3 fcolor;

uniform mat4 proj_matrix;
uniform mat4 v_matrix;
// uniform vec2 offset;
// uniform vec4 color;

void main()
{
  //float scale = 10.0f;
  // TexCoords = vertex.zw;
  // ParticleColor = color;
  gl_Position = proj_matrix * v_matrix * vec4(pos, 1.0);
  // gl_Position = vec4(0,0,-10,1.0);
  fcolor = normalize(pos) * 0.5 + vec3(0.5, 0.5, 0.5);
}