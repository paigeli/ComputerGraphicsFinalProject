#ifndef _NORMAL_GENERATOR_H_
#define _NORMAL_GENERATOR_H_

#include "Generator.h"
#include "../Models/Particle.h"

using namespace std;

class NormalGenerator : public Generator {
  public:
    NormalGenerator(json& _j);
    
    void setValues(glm::vec3& _v) override;

  private:
    std::normal_distribution<float> m_distributionX; ///< Distribution for 
                                                         ///< x value
    std::normal_distribution<float> m_distributionY; ///< Distribution for 
                                                         ///< y value
    std::normal_distribution<float> m_distributionZ; ///< Distribution for 
                                                         ///< z value
  
};

#endif