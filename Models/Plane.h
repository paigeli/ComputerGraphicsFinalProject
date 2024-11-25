#ifndef _PLANE_H
#define _PLANE_H

#include "Body.h"
#include "Mesh.h"

class Plane : public Body {
  public:
    Plane(json& _j, std::string _type);

    Collision intersects(Ray& _ray);
    void initialize(GLuint& _program);
    void draw();

    glm::mat4 m_mMatrix;

  private:
    glm::vec3 m_center, m_normal, m_scale, m_rAxis;
    float m_width, m_height, m_rDegree, m_lineWidth;
    int m_numVertices, m_numIndices;
    std::vector<Vertex> m_vertices;
    std::vector<int> indices; 
    std::vector<glm::vec3> positions, tangents;
    std::vector<glm::vec2> textures;
    GLuint m_vao{0}, m_vbo{0}, m_eb{0}, mLoc, m_texKd, m_texKs, m_normMap; ///< vao, vbo, index buffer

    void read(json& _j);
    void calculateVertices();
    void computeTangent();
};

#endif