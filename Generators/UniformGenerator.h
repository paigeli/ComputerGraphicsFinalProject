#ifndef _UNIFORM_GENERATOR_H_
#define _UNIFORM_GENERATOR_H_

#include "Generator.h"
#include "../Models/Particle.h"

using namespace std;

class UniformGenerator : public Generator {
  public:
    UniformGenerator(json& _j);
    
    void setValues(glm::vec3& _v) override;

  private:
    std::uniform_real_distribution<float> m_distributionX; ///< Distribution for 
                                                         ///< x value
    std::uniform_real_distribution<float> m_distributionY; ///< Distribution for 
                                                         ///< y value
    std::uniform_real_distribution<float> m_distributionZ; ///< Distribution for 
                                                         ///< z value
  
};

#endif