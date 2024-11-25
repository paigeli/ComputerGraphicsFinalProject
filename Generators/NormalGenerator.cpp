#include "NormalGenerator.h"

NormalGenerator::
NormalGenerator(json& _j) {
  m_distributionX = normal_distribution<float>(_j.at("xMean"), _j.at("xSd"));
  m_distributionY = normal_distribution<float>(_j.at("yMean"), _j.at("ySd"));
  m_distributionZ = normal_distribution<float>(_j.at("zMean"), _j.at("zSd"));
}

void
NormalGenerator::
setValues(glm::vec3& _v) {
  _v.x = m_distributionX(m_generator);
  _v.y = m_distributionY(m_generator);
  _v.z = m_distributionZ(m_generator);
}
