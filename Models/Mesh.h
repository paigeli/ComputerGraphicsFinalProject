#ifndef _MESH_H_
#define _MESH_H_

#include "Material.h"
#include <OpenGL/gl3.h>
#include <memory>

// SOIL2
#include <SOIL2/SOIL2.h>

////////////////////////////////////////////////////////////////////////////////
/// @brief One possible storage of vertex information.
/// TODO: need change when do texture mapping
////////////////////////////////////////////////////////////////////////////////
struct Vertex {
  glm::vec3 m_p; ///< Position
  glm::vec3 m_n; ///< Normal
  glm::vec2 m_t; ///< Texture
  glm::vec3 m_tangent; // tagents


  Vertex(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _t, const glm::vec3& _ta) :
    m_p(_p), m_n(_n), m_t(_t), m_tangent(_ta){}
  Vertex(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _t) :
    m_p(_p), m_n(_n), m_t(_t) {}
};


////////////////////////////////////////////////////////////////////////////////
/// @brief mesh data class
///
/// Vertices will be ordered such that every three form a triangle, e.g.,
/// vertices at indices 0, 1, 2 form a triangle, and then vertices at indices
/// 3, 4, 5 form a triangle, etc.
////////////////////////////////////////////////////////////////////////////////
class Mesh {
  public:
    // vertices and materials from .obj and .mtl
    
    Material m_material;            ///< Material
    glm::mat4 m_mMatrix; ///< model matrix and normal transform
    

    Mesh() = default;

    Mesh(const std::vector<Vertex>& _vertices, Material _m) : 
      m_vertices(_vertices), m_material(_m) {}

    void read(json& _j);
    void initialize(GLuint& _program);
    void draw();

  private:
    // data parse from json file
    glm::vec3 m_translate, m_rAxis, m_scale;
    float m_rDegree;
    std::vector<Vertex> m_vertices;
    GLuint m_vao{0}, m_vbo{0}, mLoc; ///< vao, vbo, index buffer
};

using MeshPtr = std::shared_ptr<Mesh>;

#endif

