#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Ray.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
using namespace std;

#include <GLFW/glfw3.h> 
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Camera {
  public:
    Camera(json& _j);

    Ray rayGenerate(int _i, int _j);
    vector<Ray> multiRaysGen(int _i, int _j, int _n);
    void processInput(GLFWwindow* window);
    void normalize();

    glm::mat4 view, proj;
    glm::vec3 m_eye;

  private:
    int m_width, m_height;
    glm::vec3 m_u, m_v, m_w, m_lookat, m_up;
    float m_distance;
    float m_theta, m_yaw, m_pitch;
    float l, r, b, t;
    bool m_orthoView;
    float m_near, m_far;
    float cameraSpeed; // adjust accordingly
    float degreeSpeed;

    void initialize();
};

#endif