////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data. Additionally, pass along the color
// data about the vertex.
////////////////////////////////////////////////////////////////////////////////

#version 330

layout(location=0) in vec3   vpos; // Input vertex position from data
layout(location=1) in vec3   vnormal; // Input vertex color from data
layout(location=2) in vec2   vtexture; // input texture
layout(location=3) in vec3   vtangent; // tagent
out vec3   fpos;   // fragment posistion
out vec3   normal; // Assigned vertex color to send to rasterizer
out vec2   textCoord;  // texture coordinates
out vec3   tangent;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;


void main() {
  vec4 pos4 = vec4(vpos,1.f);
  vec4 norm4 = vec4(vnormal, 0.f);
  vec4 tang4 = vec4(vtangent, 0.f);
  gl_Position = proj_matrix * v_matrix * m_matrix * pos4;
  fpos = (v_matrix * m_matrix * pos4).xyz;
  normal = normalize((transpose(inverse(v_matrix*m_matrix)) * norm4).xyz);
  tangent = normalize((transpose(inverse(v_matrix*m_matrix)) * tang4).xyz);
  textCoord = vtexture;
  //color =  vpos * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
  //color = (vcolor + 1)/2;
  //color = vec4(ia * ka, 1.0);
}
