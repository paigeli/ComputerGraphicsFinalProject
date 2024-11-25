#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Models/Particle.h"
#include "GLInclude.h"
#include "Generators/UniformGenerator.h"
#include "Generators/NormalGenerator.h"
#include "Forces/Attraction.h"
#include "Forces/Gravity.h"
#include "Forces/ConstantForce.h"
#include "Forces/Repulsion.h"
#include "Forces/Drag.h"
#include <vector>

class ParticleSystem {
  public:
    ParticleSystem(json& _j);

    void initialize();
    void update(float _dt);
    void draw();

  private:
    std::vector<ParticlePtr> m_particles;
    std::vector<ForcePtr> m_forces;
    std::shared_ptr<Attraction> m_attractor;
    int m_size;
    glm::vec3 m_translate, m_rAxis, m_scale;
    float m_rDegree, m_pSize;
    GLuint m_vao{0}, m_vbo{0}, mLoc, m_texKd, m_texKs;
    std::unique_ptr<Generator> m_generator;

    void parseGenerators(json& _j);
};

#endif