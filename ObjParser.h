#ifndef _OBJ_PARSER_H_
#define _OBJ_PARSER_H_

#include "Models/Mesh.h"

class ObjParser {
  public:
    Mesh parse_obj_file(const std::string& _filename);
    void computeTangent();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<Vertex> vertices;
};

#endif