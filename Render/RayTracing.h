#ifndef _RAY_TRACING_H_
#define _RAY_TRACING_H_

#include "Render.h"

class RayTracing : public Render {
  public:
    RayTracing(json& _j, int _width, int _height);

    void draw(GLFWwindow* window) override;
    void initialize() override;

  private:
    std::unique_ptr<glm::vec4[]> m_frame{nullptr}; ///< Framebuffer
    int m_width, m_height;

    Color4 rayTracing(int _i, int _j);

    /**
    Color4 idealReflect(int _i, int _j, int _depth);
    Color4 antiAliasing(int _i, int _j, int _nRays);

    // Helper Method
    Color4 reflect(Ray& _ray, Collision& _c, int _depth);
    */

};

#endif