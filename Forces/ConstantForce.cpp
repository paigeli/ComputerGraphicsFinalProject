#include "ConstantForce.h"

ConstantForce::
ConstantForce(json& _j) : Force(_j) {
  m_force = _j.at("force");
  m_dir = glm::vec3(_j.at("direction")[0], _j.at("direction")[1], _j.at("direction")[2]);
}

glm::vec3
ConstantForce::
getForce(Particle& _p) {
  return m_force * m_dir * m_coefficient;
}