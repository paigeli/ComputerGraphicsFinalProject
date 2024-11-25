#ifndef _GRAVITY_H_
#define _GRAVITY_H_

#include "Force.h"

class Gravity : public Force {
  public:
    Gravity(json& _j) : Force(_j){}

    glm::vec3 getForce(Particle& _p) {return g*glm::vec3(0,-1.f,0)* m_coefficient;}
};

#endif