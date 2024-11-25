#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include "ObjParser.h"
#include "Models/Material.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief Parse an obj file into a mesh
/// @param _filename Filename
/// @return Loaded mesh
Mesh
ObjParser::
parse_obj_file(const std::string& _filename) {
  std::ifstream ifs(_filename);
  std::string mtllib, mtl;
  if(!ifs) {
    return Mesh();
  }
  //std::cout << "Parsing: " << _filename << std::endl;

  

  std::string line;
  while(getline(ifs, line)) {
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;

    if(tag == "v") {
      glm::vec3 p;
      iss >> p.x >> p.y >> p.z;
      positions.emplace_back(p);
    }
    else if(tag == "vt") {
      glm::vec2 t;
      iss >> t.x >> t.y;
      textures.emplace_back(t);
    }
    else if(tag == "vn") {
      glm::vec3 n;
      iss >> n.x >> n.y >> n.z;
      normals.emplace_back(n);
    }
    else if(tag == "f") {
      for(size_t i = 0; i < 3; ++i) {
        std::string vert;
        iss >> vert;
        size_t p, t, n;
        sscanf(vert.c_str(), "%zu/%zu/%zu", &p, &t, &n);
        vertices.emplace_back(positions[p-1], normals[n-1], textures[t-1]);
      }
    }
    else if (tag == "mtllib"){
      iss >> mtllib;
    }
    else if (tag == "usemtl"){
      iss >> mtl;
    }
  }

  computeTangent();

  // // debug info
  // std::cout << "Number of positions: " <<    positions.size() << std::endl;
  // std::cout << "Number of  textures: " <<     textures.size() << std::endl;
  // std::cout << "Number of   normals: " <<      normals.size() << std::endl;
  // std::cout << "Number of   tangents: " <<      tangents.size() << std::endl;
  // std::cout << "Number of     faces: " << (vertices.size()/3) << std::endl;

  return Mesh(vertices, Material(mtllib, mtl));
}

void
ObjParser::
computeTangent() {
  for ( int i=0; i<vertices.size(); i+=3){
    // Shortcuts for vertices
    glm::vec3 & v0 = vertices[i+0].m_p;
    glm::vec3 & v1 = vertices[i+1].m_p;
    glm::vec3 & v2 = vertices[i+2].m_p;

    // Shortcuts for UVs
    glm::vec2 & uv0 = vertices[i+0].m_t;
    glm::vec2 & uv1 = vertices[i+1].m_t;
    glm::vec2 & uv2 = vertices[i+2].m_t;

    // Edges of the triangle : position delta
    glm::vec3 deltaPos1 = v1-v0;
    glm::vec3 deltaPos2 = v2-v0;

    // UV delta
    glm::vec2 deltaUV1 = uv1-uv0;
    glm::vec2 deltaUV2 = uv2-uv0;
    
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    vertices[i+0].m_tangent = tangent;
    vertices[i+1].m_tangent = tangent;
    vertices[i+2].m_tangent = tangent;
  }
}