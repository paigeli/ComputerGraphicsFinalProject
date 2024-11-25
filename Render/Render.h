#ifndef _RENDER_H_
#define _RENDER_H_

#include <string>
#include "../Scene.h"
#include "../ParticleSystem.h"

#include "../GLInclude.h"

class Render {
  public:
    Render(json& _j) {
      m_tag = _j.at("RenderOption");
      m_scene = std::make_shared<Scene>(_j);
    }

    virtual void draw(GLFWwindow* window) = 0;
    virtual void initialize() {}
    virtual void update(float _dt) {}

    const std::string& getTag() {return m_tag;}

    int m_width, m_height;
    std::shared_ptr<Scene> m_scene;
    std::vector<std::shared_ptr<ParticleSystem>> m_particleSystems;

  private:
    std::string m_tag;

};


#endif