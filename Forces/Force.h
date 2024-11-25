#ifndef _FORCE_H_
#define _FORCE_H_

#include "../Models/Particle.h"
#include <glm/glm.hpp>
#include <string>

class Force {
  public:
    Force(json& _j) {
      m_name = _j.at("Name");
      m_coefficient = _j.at("Coefficient");}

    const float G = 6.674e-11f;
    const float g = 9.8;
    float m_coefficient;

    virtual glm::vec3 getForce(Particle& _p) = 0;

  private:
    float m_force;
    std::string m_name;
    
};

using ForcePtr = std::shared_ptr<Force>;

#endif