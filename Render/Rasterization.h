#ifndef _RASTERIZATION_H_
#define _RASTERIZATION_H_

#include "Render.h"

class Rasterization : public Render {
  public:
    Rasterization(json& _j);

    void draw(GLFWwindow* window) override;
    void initialize() override;
    void update(float _dt) override;

  private:
    GLuint iaLoc;
    glm::vec3 m_ia, m_fogColor;
    GLuint vLoc, vLoc1,vLoc2, projLoc1, projLoc, projLoc2, vPosLoc, fogDensityLoc, fogColorLoc;    ///< camera's eye
    GLuint m_program{0}, m_lampProg{0}, m_particleProg{0};
    std::string m_vertShader, m_fragShader;
    float m_fogDensity;
    void processInput(GLFWwindow* window);


};

#endif