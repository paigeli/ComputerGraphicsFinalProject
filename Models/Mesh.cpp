#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

void
Mesh::
read(json& _j) {
  m_translate = glm::vec3(_j.at("translate")[0], _j.at("translate")[1], _j.at("translate")[2]);
  m_rAxis = glm::vec3(_j.at("rotateAxis")[0], _j.at("rotateAxis")[1], _j.at("rotateAxis")[2]);
  m_scale = glm::vec3(_j.at("scale")[0], _j.at("scale")[1], _j.at("scale")[2]);
  m_rDegree = _j.at("rotateDegree");
  m_mMatrix = glm::translate(glm::mat4(1.0f), m_translate);
  m_mMatrix = glm::rotate(m_mMatrix, glm::radians(m_rDegree), m_rAxis);
  m_mMatrix = glm::scale(m_mMatrix, m_scale);
}

void
Mesh::
initialize(GLuint& _program) {
  // Material 
  
  m_material.initialize(_program);
  mLoc = glGetUniformLocation(_program, "m_matrix");

  size_t g_n = m_vertices.size();

  // Generate vertex array
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);


  // Generate/specify vertex buffer
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*g_n, &m_vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (char*)NULL + 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (char*)NULL + sizeof(glm::vec3));
  // texture                      
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (char*)NULL + sizeof(glm::vec3) + sizeof(glm::vec3));
  // tagents
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (char*)NULL + sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2));
  
   
}

void
Mesh::
draw() {
  // Material
  m_material.sendUniform();

  glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m_mMatrix));

  
  //////////////////////////////////////////////////////////////////////////////
  // Draw
  glBindVertexArray(m_vao);

  //glEnable(GL_DEPTH_TEST); 
  //glDepthFunc(GL_LEQUAL);

  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
  glBindVertexArray(0);


  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh->m_eb);
  // glDrawElements(GL_TRIANGLES, g_n, GL_UNSIGNED_INT, 0);
}