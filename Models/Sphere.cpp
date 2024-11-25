#include "Sphere.h"
#include <math.h>
#include <iostream>

Sphere::
Sphere(json& _j, std::string _type) : Body(_type) {
  read(_j);
}

Collision
Sphere::
intersects(Ray& _ray){
  float a, b, c, delta;
  glm::vec3 d = _ray.getDirection();
  glm::vec3 p = _ray.getPoint();
  Collision collide{-1.f};

  // at^2 + bt + c
  a = glm::dot(d, d);
  b = 2 * glm::dot(d, (p-m_center));
  c = glm::dot((p-m_center), (p-m_center)) - m_radius * m_radius;
  delta = b * b - 4 * a * c;
  if (delta >= 0) {
    collide.m_t = (-b-sqrt(delta))/(2*a);
    if (collide.m_t < 0)
      collide.m_t = (-b+sqrt(delta))/(2*a); // Need smallest greater than t.
    if (collide.m_t > 0.1) {
      collide.isCollided = true;
      collide.m_hit = _ray.at(collide.m_t);
      collide.m_v = -_ray.getDirection();
      collide.m_normal = glm::normalize(((collide.m_hit - m_center) / m_radius));
    }
  }

  return collide;
}

void
Sphere::
read(json& _j) {
  // get material
  m_material = Material(_j.at("material"));
  // center
  m_center = glm::vec3(_j.at("center")[0], _j.at("center")[1],_j.at("center")[2]);
  // radius
  m_radius = _j.at("radius");
  // precision
  int prec = _j.at("precision");
  calculateVertices(prec);
  // scale
  m_scale = glm::vec3(_j.at("scale")[0], _j.at("scale")[1], _j.at("scale")[2]);
  
  m_mMatrix = glm::translate(glm::mat4(1.0f), m_center);
  //m_mMatrix = glm::rotate(m_mMatrix, glm::radians(m_rDegree), m_rAxis);
  m_mMatrix = glm::scale(m_mMatrix, m_scale);
}

void
Sphere::
calculateVertices(int prec) {
  m_numVertices = (prec + 1) * (prec + 1);
  m_numIndices = prec * prec * 6;

  for (int i = 0; i < m_numVertices; i++) { positions.emplace_back(glm::vec3()); }
  for (int i = 0; i < m_numVertices; i++) { textures.emplace_back(glm::vec2()); }
  for (int i = 0; i < m_numVertices; i++) { normals.emplace_back(glm::vec3()); }
  for (int i = 0; i < m_numVertices; i++) { tangents.emplace_back(glm::vec3()); }
  for (int i = 0; i < m_numIndices; i++) { indices.emplace_back(0); } // increases the vector size by 1
  // calculate triangle vertices
  for (int i = 0; i <= prec; ++i) {
    for (int j = 0; j <= prec; ++j) {
      using namespace glm;
      float y = (float)cos(glm::radians(180.0f - i * 180.0f / prec));
      float x = -(float)cos(glm::radians(j*360.0f / prec)) * (float)abs(cos(asin(y))); 
      float z = (float)sin(glm::radians(j*360.0f / prec)) * (float)abs(cos(asin(y))); 
      positions[i*(prec + 1) + j] = glm::vec3(x, y, z);
      textures[i*(prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec)); 
      normals[i*(prec + 1) + j] = normalize(glm::vec3(x,y,z));
      // calculate tangent vector
      if (((x==0) && (y==1) && (z==0)) || ((x==0) && (y==-1) && (z==0))) // if north or south pole, { 
        tangents[i*(prec+1)+j] = glm::vec3(0.0f, 0.0f, -1.0f); // set tangent to -Z axis }
      else // otherwise, calculate tangent       
        tangents[i*(prec+1)+j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x,y,z));
    }
  }
  // calculate triangle indices
  for (int i = 0; i<prec; i++) {
    for (int j = 0; j<prec; j++) {
      indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j; 
      indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1; 
      indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j; 
      indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1; 
      indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1; 
      indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
    } 
  }

  for (size_t i=0; i<m_numIndices; ++i) {
    m_vertices.emplace_back(positions[indices[i]], normals[indices[i]], textures[indices[i]], tangents[indices[i]]);
  }

  // std::cout << "Number of positions: " <<    positions.size()
  // << "First element: " << positions[0].x 
  // << "\nSecond: " << positions[0].y 
  // << "\nThird: " << positions[0].z << std::endl;
  // std::cout << "Number of  textures: " <<     textures.size() << std::endl;
  // std::cout << "Number of   normals: " <<      normals.size() << std::endl;
  // std::cout << "Number of   tangents: " <<      tangents.size() << std::endl;
  // std::cout << "Number of     faces: " << (m_vertices.size()/3) << std::endl;
  
}

void
Sphere::
initialize(GLuint& _program) {
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
Sphere::
draw() {
  // Material
  m_material.sendUniform();

  glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m_mMatrix));
  
  //////////////////////////////////////////////////////////////////////////////
  // Draw
  glBindVertexArray(m_vao);


  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
  glBindVertexArray(0);

}