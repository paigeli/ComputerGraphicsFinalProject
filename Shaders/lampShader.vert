#version 330

layout(location=0) in vec3   vpos; // Input vertex position from data


uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;


void main() {
  vec4 pos4 = vec4(vpos,1.f);
  gl_Position = proj_matrix * v_matrix * m_matrix * pos4;
}