#include "ParticleSystem.h"
#include <chrono>

using my_clock = std::chrono::high_resolution_clock;
using seconds = std::chrono::duration<float>;


ParticleSystem::
ParticleSystem(json& _j) {
  m_pSize = _j.at("Psize");
  m_size = _j.at("Size");
  parseGenerators(_j.at("Generator"));
  m_attractor = std::make_shared<Attraction>(_j.at("Attraction"));
  m_forces.emplace_back(new Gravity(_j.at("Gravity")));
  m_forces.emplace_back(new Drag(_j.at("Drag")));
  m_forces.emplace_back(new ConstantForce(_j.at("ConstantForce")));
  m_forces.emplace_back(new Repulsion(_j.at("Repulsion")));
}

void
ParticleSystem::
initialize() {
  for (int i=0; i<m_size; ++i){
    m_particles.emplace_back(new Particle());
  }

  for (auto& p : m_particles){
    m_generator->setValues(p->m_pos);
    //m_generator->setValues(p->m_vel);
    //p->m_vel = glm::vec3(0,-1,0);
  }
  // for (int i=0; i<5; ++i) {
  //   for (int j=0; j<5; ++j) {
  //     m_particles[i*5+j]->m_pos = glm::vec3(i,j,-10);
  //   }
  // }

  // Generate vertex array
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // size_t n = m_particles.size();

  // Generate/specify vertex buffer
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*n, &m_particles[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        0, (char*)NULL + 0);
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
  //                       sizeof(Vertex), (char*)NULL + sizeof(glm::vec3));
  // // texture                      
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
  //                       sizeof(Vertex), (char*)NULL + sizeof(glm::vec3) + sizeof(glm::vec3));
}

void
ParticleSystem::
draw() {
  glPointSize(m_pSize);
  glBindVertexArray(m_vao);

  //glEnable(GL_DEPTH_TEST); 
  //glDepthFunc(GL_LEQUAL);
  size_t n = m_particles.size();

  vector<glm::vec3> positions;
  for (auto& p : m_particles) {
      positions.emplace_back(p->m_pos);

  }
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*n, &positions[0], GL_STATIC_DRAW);
  glDrawArrays(GL_POINTS, 0, n);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
}

void
ParticleSystem::
update(float _dt) {
  for (auto& p : m_particles) {
    if (p->m_lifeFrame >=0){
      // Force compute acceleration
      p->m_acc = m_attractor->getForce(*p);
      for (auto& f : m_forces){
        p->m_acc += f->getForce(*p);
      }
      // Velocity
      p->m_vel = p->m_vel + p->m_acc * _dt;

      // position
      p->m_pos = p->m_pos + p->m_vel * _dt;

    }
    else {
      p->m_vel = glm::vec3(0,0,0);
      
    }
    p->m_lifeFrame--;
  }

   
}

void 
ParticleSystem::
parseGenerators(json& _j) {
  if (_j.at("Type") == "Normal")
    m_generator = std::make_unique<NormalGenerator>(_j);
  else if (_j.at("Type") == "Uniform")
    m_generator = std::make_unique<UniformGenerator>(_j);
}