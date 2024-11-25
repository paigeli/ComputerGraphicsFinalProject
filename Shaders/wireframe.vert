////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data. Additionally, pass along the color
// data about the vertex.
////////////////////////////////////////////////////////////////////////////////

#version 330

in vec3   vpos; // Input vertex position from data
in vec3   vnormal; // Input vertex color from data

out vec4   color; 

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform vec3 ka, ia;


void main() {
  gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vpos, 1.f);

  //color =  vec4(vpos, 1.f) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
  color = (vec4(normalize(vnormal),1.f) + 1)/2;
  //color = vec4(ia * ka, 1.0);
}