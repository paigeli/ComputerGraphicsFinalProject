#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Body.h"
#include "Mesh.h"

class Sphere : public Body {
  public:
    Sphere(json& _j, std::string _type);
    
    Collision intersects(Ray& _ray);
    void initialize(GLuint& _program);
    void draw();

    glm::mat4 m_mMatrix;

  private:
    glm::vec3 m_center, m_scale;
    float m_radius; ///< implementation makes it default as 1. use scale to scale
    int m_numVertices, m_numIndices;
    std::vector<Vertex> m_vertices;
    std::vector<int> indices; 
    std::vector<glm::vec3> positions, normals, tangents;
    std::vector<glm::vec2> textures;
    GLuint m_vao{0}, m_vbo{0}, m_eb{0}, mLoc, m_texKd, m_texKs, m_normMap; ///< vao, vbo, index buffer

    void read(json& _j);
    void calculateVertices(int prec);
};

#endif