#include "UniformGenerator.h"

UniformGenerator::
UniformGenerator(json& _j) {
  m_distributionX = uniform_real_distribution<float>(_j.at("xMin"), _j.at("xMax"));
  m_distributionY = uniform_real_distribution<float>(_j.at("yMin"), _j.at("yMax"));
  m_distributionZ = uniform_real_distribution<float>(_j.at("zMin"), _j.at("zMax"));
}

void
UniformGenerator::
setValues(glm::vec3& _v) {
  _v.x = m_distributionX(m_generator);
  _v.y = m_distributionY(m_generator);
  _v.z = m_distributionZ(m_generator);
}
