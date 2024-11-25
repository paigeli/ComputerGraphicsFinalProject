#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>
#include "Sphere.h"
#include <memory>
#include <vector>

enum Type {
  Directional,
  Point,
  Spotlight
};


class Light {
  public:
    Light(json& _j);
    
    glm::vec3 processLight(Collision _c, BodyPtr _object, std::vector<BodyPtr> _objs);

    void initialize(GLuint& _program, size_t _i);

    glm::vec3 m_pos; ///< position
    glm::vec3 m_ia, m_id, m_is; ///< ambient, diffuse, specular intensity

  private:
    GLuint /*iaLoc,*/ idLoc, isLoc, lPosLoc, typeLoc,
      acLoc, alLoc, aqLoc, aaLoc, cutoffLoc, dirLoc;  ///< Light uniforms
    Type m_type;
    GLuint lightsLoc;
    glm::vec3 m_dir{0.f, -1.f, 0.f};
    float m_ac, m_al, m_aq, m_aa{0.f};       ///< attenuation constants
    float m_cutoff = 180.f;
};

using LightPtr = std::shared_ptr<Light>;

#endif