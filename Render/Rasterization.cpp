#include "Rasterization.h"
#include "../CompileShaders.h"

Rasterization::
Rasterization(json& _j) : Render(_j) {
  json j = _j.at("Options");
  // Global ambient TODO: put into Scene
  m_ia = glm::vec3(j.at("ia")[0], j.at("ia")[1], j.at("ia")[2]);
  m_vertShader = j.at("vertShader");
  m_fragShader = j.at("fragShader");
  m_fogColor = glm::vec3(j.at("fogColor")[0], j.at("fogColor")[1], j.at("fogColor")[2]);
  m_fogDensity = j.at("fogDensity");
  for (auto& j : _j.at("ParticleSystem")) {
    m_particleSystems.emplace_back(new ParticleSystem(j));
  }
}

void
Rasterization::
initialize() {
  glClearColor(0.f, 0.f, 0.f, 0.f);
  // glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  // glDisable(GL_DEPTH_TEST);

  // glEnable(GL_BLEND);  
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  
  m_program = compileProgram(m_vertShader, m_fragShader);
  m_lampProg = compileProgram("Shaders/lampShader.vert", "Shaders/lampShader.frag");
  m_particleProg = compileProgram("Shaders/Particles.vert", "Shaders/Particles.frag");

  glUseProgram(m_program);
  iaLoc = glGetUniformLocation(m_program, "ia");
  fogDensityLoc = glGetUniformLocation(m_program, "fogDensity");
  fogColorLoc = glGetUniformLocation(m_program, "fogColor");
  glUniform3fv(iaLoc, 1, glm::value_ptr(m_ia));
  glUniform3fv(fogColorLoc, 1, glm::value_ptr(m_fogColor));
  glUniform1f(fogDensityLoc, m_fogDensity);

  // uniform locations
  vLoc = glGetUniformLocation(m_program, "v_matrix");
  projLoc = glGetUniformLocation(m_program, "proj_matrix");
  vPosLoc = glGetUniformLocation(m_program, "vPos");

  auto& lights = m_scene->getLights();
  for (size_t i=0; i<lights.size(); ++i) {
    lights[i]->initialize(m_program, i);
  }

  // objects
  for (auto& m_obj : m_scene->getObjects()){
    m_obj->initialize(m_program);
  }

  // windows
  // for (auto& m_obj : m_scene->getWindows()){
  //   m_obj->initialize(m_program);
  // }


  // Mesh
  for (auto& m_mesh : m_scene->getMesh()) {
    m_mesh->initialize(m_program);
  }

  glUseProgram(m_lampProg);
  vLoc2 = glGetUniformLocation(m_lampProg, "v_matrix");
  projLoc2 = glGetUniformLocation(m_lampProg, "proj_matrix");
  // lamps
  for (auto& m_mesh : m_scene->getLamps()) {
    m_mesh->initialize(m_lampProg);
  }

  glUseProgram(m_particleProg);
  vLoc1 = glGetUniformLocation(m_particleProg, "v_matrix");
  projLoc1 = glGetUniformLocation(m_particleProg, "proj_matrix");
  for (auto& ps : m_particleSystems){
    ps->initialize();
  }
}

void
Rasterization::
draw(GLFWwindow* window) {
  // glUseProgram(m_particleProg);
  // clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  // Camera
  auto c = m_scene->getCamera();
  c->processInput(window);
  
  // glUniformMatrix4fv(vLoc1, 1, GL_FALSE, glm::value_ptr(c->view));
  
  

  // Mode
  processInput(window);

  glUseProgram(m_lampProg);
  glUniformMatrix4fv(vLoc2, 1, GL_FALSE, glm::value_ptr(c->view));
  glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(c->proj));
  for (auto& m_mesh : m_scene->getLamps()) {
    m_mesh->draw();
  }

  glUseProgram(m_particleProg);
  glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(c->proj));
  glUniformMatrix4fv(vLoc1, 1, GL_FALSE, glm::value_ptr(c->view));
  for (auto& ps : m_particleSystems){
    ps->draw();
  }

  glUseProgram(m_program);
  glUniform3fv(vPosLoc, 1, glm::value_ptr(c->m_eye));
  glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(c->view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(c->proj));

  // glDisable(GL_DEPTH_TEST);
  for (auto& m_mesh : m_scene->getMesh()) {
    m_mesh->draw();
  }

    // objects
  for (auto& m_obj : m_scene->getObjects()){
    m_obj->draw();
  }

  // glEnable(GL_DEPTH_TEST);

  // for (auto& m_obj : m_scene->getWindows()){
  //   m_obj->draw();
  // }


}

void
Rasterization::
processInput(GLFWwindow* window) {
  // wire
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  } else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  // back culling
  // if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
  //   glEnable(GL_CULL_FACE);
  //   glCullFace(GL_FRONT);
  // } 
  // else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
  //   glEnable(GL_CULL_FACE);
  //   glCullFace(GL_BACK);
  // }
  // else {
  //   glDisable(GL_CULL_FACE);
  // }
}

void
Rasterization::
update(float _dt) {
  for (auto& ps : m_particleSystems){
    ps->update(_dt);
  }
}