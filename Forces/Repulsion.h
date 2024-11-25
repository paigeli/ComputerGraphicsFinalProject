#ifndef _REPULSION_H_
#define _REPULSION_H_

#include "Force.h"

class Repulsion : public Force {
  public:
    Repulsion(json& _j) : Force(_j) {m_goal = glm::vec3(_j.at("goal")[0], _j.at("goal")[1], _j.at("goal")[2]);}

    glm::vec3 getForce(Particle& _p) {
      glm::vec3 u = -(m_goal - _p.m_pos);
      float r = glm::length(u);
      return (g/(r*r))*u*m_coefficient;
    }
    void setGoal(glm::vec3 _g) {m_goal = _g;}
  private: 
    glm::vec3 m_goal;
};

#endif
