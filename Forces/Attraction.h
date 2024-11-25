#ifndef _ATTRACTION_H_
#define _ATTRACTION_H_

#include "Force.h"

class Attraction : public Force {
  public:
    Attraction(json& _j);

    // Assume Particle's mass as 1
    // @Param _p1 = the particle gets force
    // _p2 = particle 2
    // @return the force as accerator because F = ma
    glm::vec3 getForce(Particle& _p1);

    void setGoal(glm::vec3 _goal) {m_goal = _goal;}

    private:
      glm::vec3 m_goal;

};

#endif