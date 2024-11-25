#ifndef _COLLISION_H_
#define _COLLISION_H_

struct Collision {
  float m_t;
  glm::vec3 m_normal, m_hit, m_v;
  bool isCollided = false;
};

#endif