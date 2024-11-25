#ifndef _CONSTANT_FROCE_H_
#define _CONSTANT_FROCE_H_

#include "Force.h"

class ConstantForce : public Force {
  public:
    ConstantForce(json& _j);

    glm::vec3 getForce(Particle& _p) override;

  private:
    float m_force;
    glm::vec3 m_dir;
};

#endif