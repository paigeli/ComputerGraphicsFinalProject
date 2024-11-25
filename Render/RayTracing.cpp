#include "RayTracing.h"

RayTracing::
RayTracing(json& _j, int _width, int _height) : Render(_j) {
  m_frame = std::make_unique<glm::vec4[]>(_width*_height);
  m_width = _width;
  m_height = _height;
}

void
RayTracing::
initialize() {
  glClearColor(0.f, 0.f, 0.0f, 0.f);
}

void
RayTracing::
draw(GLFWwindow* window) {
  //////////////////////////////////////////////////////////////////////////////
  // Clear
  for(int i = 0; i < m_width*m_height; ++i)
    m_frame[i] = glm::vec4(0.f, 0.f, 0.f, 0.f);

  //////////////////////////////////////////////////////////////////////////////
  // Draw
  for (int i=0; i<m_width; ++i) {
    for (int j=0; j< m_height; ++j) {
      /*if (g_idealReflect)
        m_frame[j*m_width+i] = idealReflect(i, j, g_reflectDepth);
      else if (g_antialiasing)
        m_frame[j*m_width+i] = antiAliasing(i, j, g_nRays);
      else*/
        m_frame[j*m_width+i] = rayTracing(i, j);
    }
  }

  glDrawPixels(m_width, m_height, GL_RGBA, GL_FLOAT, m_frame.get());

  auto c = m_scene->getCamera();
  c->processInput(window);

  std::cout << "past drawing" << std::endl;
}


Color4
RayTracing::
rayTracing(int _i, int _j) {
  //generate rays in perspective view
  Ray ray = m_scene->getCamera()->rayGenerate(_i, _j);
  Color4 color(0, 0, 0, 1.f);

  // intersects with each object, find the smallest t
  size_t objIndex = -1;
  Collision cMin{std::numeric_limits<float>::infinity()};
  for (size_t i=0; i<m_scene->getObjects().size(); ++i){
    Collision c = m_scene->getObjects()[i]->intersects(ray);
    if (c.m_t > 0 && c.m_t < cMin.m_t) {
      cMin = c;
      objIndex = i;
    }
  }
  // for each object, process light
  if (cMin.isCollided) {
    glm::vec3 L(0,0,0);
    // add all the lights
    for (size_t j=0; j<m_scene->getLights().size(); ++j) {
      L += m_scene->getLights()[j]->processLight(cMin, m_scene->getObjects()[objIndex], m_scene->getObjects());
    }
    L = glm::min(L, glm::vec3(1,1,1));
    color = glm::vec4(L,1);
  }

  return color;
}

/*
Color4
RayTracing:: 
idealReflect(int _i, int _j, int _depth) {
  //generate rays in perspective view
  Ray ray = m_camera.rayGenerate(_i, _j);
  Color4 color(0, 0, 0, 1.f);

  // intersects with each object, find the smallest t
  size_t objIndex = -1;
  Collision cMin{std::numeric_limits<float>::infinity()};
  for (size_t i=0; i<m_objects.size(); ++i){
    Collision c = m_objects[i]->intersects(ray);
    if (c.m_t > 0.1 && c.m_t < cMin.m_t) {
      cMin = c;
      objIndex = i;
    }
  }
  // for each object, process light
  float factor = 0;
  if (cMin.isCollided) {
    glm::vec3 L(0,0,0);
    // add all the lights
    for (size_t j=0; j<m_lights.size(); ++j) {
      L += m_lights[j]->processLight(cMin, m_objects[objIndex], m_objects);
    }
    L = glm::min(L, glm::vec3(1,1,1));
    color = glm::vec4(L,1);
    factor = m_objects[objIndex]->getMaterial().m_reflect;
  }

  return color + factor*reflect(ray, cMin, _depth);
}

Color4
RayTracing:: 
reflect(Ray& _ray, Collision& _c, int _depth) {
  // Base case
  if (_depth == 0)
    return Color4(0,0,0,0);
  
  glm::vec3 newDir = _ray.getDirection() - (float)2*(glm::dot(_ray.getDirection(), _c.m_normal))
    *_c.m_normal;
  
  _ray = Ray(_c.m_hit, newDir);

  Color4 color(0, 0, 0, 1.f);

  // intersects with each object, find the smallest t
  size_t objIndex = -1;
  Collision cMin{std::numeric_limits<float>::infinity()};
  for (size_t i=0; i<m_objects.size(); ++i){
    Collision c = m_objects[i]->intersects(_ray);
    if (c.m_t > 0.1 && c.m_t < cMin.m_t) {
      cMin = c;
      objIndex = i;
    }
  }

  // for each object, process light
  float factor = 0;
  if (cMin.isCollided) {
    glm::vec3 L(0,0,0);
    // add all the lights
    for (size_t j=0; j<m_lights.size(); ++j) {
      L += m_lights[j]->processLight(cMin, m_objects[objIndex], m_objects);
    }
    L = glm::min(L, glm::vec3(1,1,1));
    color = glm::vec4(L,1);
    factor = m_objects[objIndex]->getMaterial().m_reflect;
  }


  
  return color + factor*reflect(_ray, cMin, --_depth);
}

Color4 
RayTracing::
antiAliasing(int _i, int _j, int _nRays) {
  vector<Ray> rays = m_camera.multiRaysGen(_i, _j, _nRays);
  Color4 colorSum(0, 0, 0, 1.f);
  for (size_t r=0; r<rays.size(); ++r) {
    Color4 color(0, 0, 0, 1.f);
    // intersects with each object, find the smallest t
    size_t objIndex = -1;
    Collision cMin{std::numeric_limits<float>::infinity()};
    for (size_t i=0; i<m_objects.size(); ++i){
      Collision c = m_objects[i]->intersects(rays[r]);
      if (c.m_t > 0.1 && c.m_t < cMin.m_t) {
        cMin = c;
        objIndex = i;
      }
    }
    // for each object, process light
    if (cMin.isCollided) {
      glm::vec3 L(0,0,0);
      // add all the lights
      for (size_t j=0; j<m_lights.size(); ++j) {
        L += m_lights[j]->processLight(cMin, m_objects[objIndex], m_objects);
      }
      //L = glm::min(L, glm::vec3(1,1,1));
      color = glm::vec4(L,1);
    }
    colorSum += color;
  }

  colorSum /= float(_nRays);
  colorSum = min(colorSum, glm::vec4(1,1,1,1));

  return colorSum;


}

**/