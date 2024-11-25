#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../GLInclude.h"

class Material {
  public:
    glm::vec3 m_ka, m_kd, m_ks;     ///< ambient mterial property
    float m_n;      ///< exponent for specular highlight size
    float m_reflect;
    std::string mapKd, mapKs, mapNM, mapDM, mapEM;

    Material() = default;

    Material(json& _j);

    Material(const std::string& _filename, const std::string& _mName);

    void initialize(GLuint& _program);
    void sendUniform();

  private:
    float m_depthScale{0.1};
    GLuint kaLoc, kdLoc, ksLoc, nLoc, texKdLoc, texKsLoc, texNMLoc, texDMLoc, texEMLoc;
    GLuint dScaleLoc; 
    GLuint m_texKd, m_texKs, m_normMap, m_depthMap, m_emissionMap;
    
};

#endif