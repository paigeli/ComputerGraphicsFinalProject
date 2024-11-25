#include "Camera.h"
#include <math.h>

Camera::
Camera(json& _j) {
  m_eye = glm::vec3(_j.at("eye")[0], _j.at("eye")[1], _j.at("eye")[2]);
  m_u = glm::vec3(_j.at("u")[0], _j.at("u")[1], _j.at("u")[2]);
  m_v = glm::vec3(_j.at("v")[0], _j.at("v")[1], _j.at("v")[2]);
  m_lookat = glm::vec3(_j.at("lookat")[0], _j.at("lookat")[1], _j.at("lookat")[2]);
  m_up = glm::vec3(_j.at("up")[0], _j.at("up")[1], _j.at("up")[2]);
  m_distance = _j.at("d");
  m_theta = _j.at("theta");
  m_width = _j.at("width");
  m_height = _j.at("height");
  m_orthoView = _j.at("orthographic");
  m_near = _j.at("near");
  m_far = _j.at("far");
  m_yaw = _j.at("yaw");
  m_pitch = _j.at("pitch");
  cameraSpeed = _j.at("cs");
  degreeSpeed = _j.at("ds");
  initialize();
}

void
Camera::
initialize() {
  //compute m_w
  m_w = glm::cross(m_u, m_v);
  //normalize();
  // compute l, r, b, t
  t = tan(glm::radians(m_theta)/2) * m_distance;
  b = -t;
  float a = (float)m_width/ (float)m_height;
  r = t * a;
  l = -r;
  view = glm::lookAt(m_eye, m_eye + m_lookat, m_up);
  if (m_orthoView)
    proj = glm::ortho(l,r,b,t,m_near, m_far);
  else
    proj = glm::perspective(glm::radians(m_theta), a, m_near, m_far);
}

void
Camera::
normalize() {
  float a = (float)m_width/ (float)m_height;
  m_w = -m_lookat;
  m_u = glm::normalize(glm::cross(m_lookat, m_up));
  m_v = glm::cross(m_w, m_u);
  view = glm::lookAt(m_eye, m_eye + m_lookat, m_up);
  if (m_orthoView)
    proj = glm::ortho(l,r,b,t,m_near, m_far);
  else
    proj = glm::perspective(glm::radians(m_theta), a, m_near, m_far);
}

Ray
Camera::
rayGenerate(int _i, int _j) {
  float torque, sigma;
  torque = l + ((r-l)*(_i+0.5)/m_width);
  sigma = b + ((t-b)*(_j+0.5)/m_height);
  if (m_orthoView){
    glm::vec3 point = m_eye + torque*m_u + sigma*m_v;
    Ray newRay(point, -m_w);
    return newRay;
  }
  else {
    glm::vec3 dir = torque*m_u + sigma*m_v + (-m_distance)*m_w;
    Ray newRay(m_eye, glm::normalize(dir));
    return newRay;
  }
}

vector<Ray>
Camera::
multiRaysGen(int _i, int _j, int _n) {
  vector<Ray> rays;
  float torque, sigma;
  float step = 1.f/_n;
  for (size_t i=0; i<sqrt(_n); ++i){
    for (size_t j=0; j<sqrt(_n); ++j){
      torque = l + ((r-l)*(_i+step*i)/m_width);
      sigma = b + ((t-b)*(_j+step*j)/m_height);
      if (m_orthoView){
        glm::vec3 point = m_eye + torque*m_u + sigma*m_v;
        Ray newRay(point, -m_w);
        rays.emplace_back(newRay);
      }
      else {
        glm::vec3 dir = torque*m_u + sigma*m_v + (-m_distance)*m_w;
        Ray newRay(m_eye, glm::normalize(dir));
        rays.emplace_back(newRay);
      }
    }
  }
  
  return rays;
}

void
Camera::
processInput(GLFWwindow* window) {

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    // camera lookat movement 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      m_pitch += degreeSpeed;
      m_lookat.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
      m_lookat.y = sin(glm::radians(m_pitch));
      m_lookat.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      m_pitch -= degreeSpeed;
      m_lookat.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
      m_lookat.y = sin(glm::radians(m_pitch));
      m_lookat.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      m_yaw += degreeSpeed;
      m_lookat.x = cos(glm::radians(m_yaw));
      m_lookat.z = sin(glm::radians(m_yaw));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      m_yaw -= degreeSpeed;
      m_lookat.x = cos(glm::radians(m_yaw));
      m_lookat.z = sin(glm::radians(m_yaw));
    }
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
    float panning = 1.f;
    view = glm::rotate(view, glm::radians(panning), glm::vec3(0.f, -1.f, 0.f));
    // m_eye.x = view_[0][3];
    // m_eye.z = view_[2][3];
    // m_lookat.x = -view_[0][2];
    // m_lookat.z = -view_[2][2];
    // normalize();
    return;
  }
  else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      float factor = 0.9f;
      l *= factor;
      r *= factor;
      b *= factor;
      t *= factor;
      m_theta *= factor;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      float factor = 1.1f;
      l *= factor;
      r *= factor;
      b *= factor;
      t *= factor;
      m_theta *= factor;
    }
  }
  else {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      m_eye += cameraSpeed * glm::normalize(-m_w);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      m_eye -= cameraSpeed * glm::normalize(-m_w);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      m_eye -= glm::normalize(m_u) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      m_eye += glm::normalize(m_u) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      m_eye += cameraSpeed * glm::normalize(m_v);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      m_eye -= cameraSpeed * glm::normalize(m_v);
  }

  normalize();
}