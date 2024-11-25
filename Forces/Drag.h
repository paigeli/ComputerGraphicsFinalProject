#ifndef _DRAG_H_
#define _DRAG_H_

#include "Force.h"

class Drag : public Force {
  public:
   Drag(json& _j) : Force(_j) {}

   glm::vec3 getForce(Particle& _p) {
     return -_p.m_vel * m_coefficient;
   }
};

#endif