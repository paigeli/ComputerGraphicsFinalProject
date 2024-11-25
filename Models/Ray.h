#ifndef _RAY_H_
#define _RAY_H_

#include <glm/glm.hpp>

class Ray {
  public:
    // constructor
    Ray(glm::vec3 _p, glm::vec3 _d) : m_point(_p), m_direction(_d) {}

    // getters
    glm::vec3 getPoint() const {return m_point;}
    glm::vec3 getDirection() const {return m_direction;}
    glm::vec3 at(float _t) {return m_point + _t * m_direction;}
    
  private:
    glm::vec3 m_point;
    glm::vec3 m_direction;
};


#endif