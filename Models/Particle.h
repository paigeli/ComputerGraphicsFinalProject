#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "../GLInclude.h"
#include "Body.h"

class Particle {
  public:
    Particle() = default;

    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec3 m_acc;
    glm::vec4 m_color;
    int m_lifeFrame{1000};
    
};

using ParticlePtr = std::shared_ptr<Particle>;

#endif