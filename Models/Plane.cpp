#include "Plane.h"
#include <math.h>
#include <iostream>

Plane::
Plane(json& _j, std::string _type) : Body(_type) {
  read(_j);
}

Collision
Plane::
intersects(Ray& _ray){
  Collision c{-1.f, glm::vec3(0,1,0)};
  float denominator = glm::dot(_ray.getDirection(), m_normal);
  if (denominator != 0) {
    c.m_t = glm::dot((m_center - _ray.getPoint()), m_normal) / denominator;
    if (c.m_t > 0.11){
      c.isCollided = true;
      c.m_normal = m_normal;
      c.m_hit = _ray.at(c.m_t);
      c.m_v = -_ray.getDirection();
    }
  }

  return c;
}

void
Plane::
read(json& _j) {
  // get material
  m_material = Material(_j.at("material"));
  // center
  m_center = glm::vec3(_j.at("center")[0], _j.at("center")[1],_j.at("center")[2]);
  // normal
  m_normal = glm::vec3(_j.at("normal")[0], _j.at("normal")[1], _j.at("normal")[2]);
  // width
  m_width = _j.at("width");
  // height
  m_height = _j.at("height");
  // rotate axis
  m_rAxis = glm::vec3(_j.at("rotateAxis")[0], _j.at("rotateAxis")[1], _j.at("rotateAxis")[2]);
  // rotate degree
  m_rDegree = _j.at("rotateDegree");
  m_lineWidth = _j.at("lineWidth");
  // scale
  m_scale = glm::vec3(_j.at("scale")[0], _j.at("scale")[1], _j.at("scale")[2]);
  m_mMatrix = glm::translate(glm::mat4(1.0f), m_center);
  m_mMatrix = glm::rotate(m_mMatrix, glm::radians(m_rDegree), m_rAxis);
  m_mMatrix = glm::scale(m_mMatrix, m_scale);

  calculateVertices();

}

void
Plane::
calculateVertices() {
  using namespace glm;
  m_numVertices = 4;
  m_numIndices = 4;

  for (int i = 0; i < m_numVertices; i++) { positions.emplace_back(glm::vec3()); }
  for (int i = 0; i < m_numVertices; i++) { textures.emplace_back(glm::vec2()); }
  for (int i = 0; i < m_numIndices; i++) { indices.emplace_back(0); } // increases the vector size by 1
  // calculate triangle vertices
  positions[0] = vec3(-m_width/2, 0.f, -m_height/2);
  positions[1] = vec3(m_width/2, 0.f, -m_height/2);
  positions[2] = vec3(-m_width/2, 0.f, m_height/2);
  positions[3] = vec3(m_width/2, 0.f, m_height/2);

  textures[0] = vec2(0.f, 0.f);
  textures[1] = vec2(1.f, 0.f);
  textures[2] = vec2(0.f, 1.f);
  textures[3] = vec2(1.f, 1.f);

  m_vertices.emplace_back(positions[0], m_normal, textures[0]);
  m_vertices.emplace_back(positions[1], m_normal, textures[1]);
  m_vertices.emplace_back(positions[2], m_normal, textures[2]);
  m_vertices.emplace_back(positions[2], m_normal, textures[2]);
  m_vertices.emplace_back(positions[1], m_normal, textures[1]);
  m_vertices.emplace_back(positions[3], m_normal, textures[3]);

  computeTangent();

  // std::cout << "Number of positions: " <<    positions.size() << std::endl;
  // // << "First element: " << positions[0].x 
  // // << "\nSecond: " << positions[0].y 
  // // << "\nThird: " << positions[0].z << std::endl;
  // std::cout << "Number of  textures: " <<     textures.size() << std::endl;
  // std::cout << "Number of   tangents: " <<      tangents.size() << std::endl;
  // std::cout << "Number of     faces: " << (m_vertices.size()/3) << std::endl;
  
}

void
Plane::
initialize(GLuint& _program) {
  glLineWidth(m_lineWidth);
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
Plane::
draw() {
  // Material
  m_material.sendUniform();

  glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m_mMatrix));
  
  //////////////////////////////////////////////////////////////////////////////
  // Draw
  glBindVertexArray(m_vao);

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  
  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
  glBindVertexArray(0);

}

void
Plane::
computeTangent() {
  for ( int i=0; i<m_vertices.size(); i+=3){
    // Shortcuts for vertices
    glm::vec3 & v0 = m_vertices[i+0].m_p;
    glm::vec3 & v1 = m_vertices[i+1].m_p;
    glm::vec3 & v2 = m_vertices[i+2].m_p;

    // Shortcuts for UVs
    glm::vec2 & uv0 = m_vertices[i+0].m_t;
    glm::vec2 & uv1 = m_vertices[i+1].m_t;
    glm::vec2 & uv2 = m_vertices[i+2].m_t;

    // Edges of the triangle : position delta
    glm::vec3 deltaPos1 = v1-v0;
    glm::vec3 deltaPos2 = v2-v0;

    // UV delta
    glm::vec2 deltaUV1 = uv1-uv0;
    glm::vec2 deltaUV2 = uv2-uv0;
    
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    m_vertices[i+0].m_tangent = tangent;
    m_vertices[i+1].m_tangent = tangent;
    m_vertices[i+2].m_tangent = tangent;
  }
}