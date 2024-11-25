#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <random>
#include <glm/glm.hpp>

class Generator {
  public:
    std::default_random_engine m_generator; 

    virtual void setValues(glm::vec3& _v) = 0;
};



#endif