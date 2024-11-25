#include "Attraction.h"

Attraction::
Attraction(json& _j) : Force(_j) {
  m_goal = glm::vec3(_j.at("goal")[0], _j.at("goal")[1], _j.at("goal")[2]);
}

glm::vec3
Attraction::
getForce(Particle& _p) {
  glm::vec3 u = -(m_goal - _p.m_pos);
  float r = glm::length(u);
  
  return -(g/(r*r))*u*m_coefficient;
}