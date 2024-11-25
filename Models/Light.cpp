#include "Light.h"

#include <math.h>
#include <iostream>

Light::
Light(json& _j) {
  // for ray tracing
  m_ia = glm::vec3(_j.at("ia")[0], _j.at("ia")[1], _j.at("ia")[2]);
  
  std::string type = _j.at("Type");

  if (type == "Directional") {
    m_type = Type::Directional;
    m_dir = glm::vec3(_j.at("direction")[0], _j.at("direction")[1],_j.at("direction")[2]);
    m_id = glm::vec3(_j.at("id")[0],_j.at("id")[1],_j.at("id")[2]);
    m_is = glm::vec3(_j.at("is")[0],_j.at("is")[1],_j.at("is")[2]);
  }
  else if (type == "Point") {
    m_type = Type::Point;
    m_pos = glm::vec3(_j.at("position")[0], _j.at("position")[1],_j.at("position")[2]);
    m_id = glm::vec3(_j.at("id")[0],_j.at("id")[1],_j.at("id")[2]);
    m_is = glm::vec3(_j.at("is")[0],_j.at("is")[1],_j.at("is")[2]);
    m_ac = _j.at("ac");
    m_al = _j.at("al");
    m_aq = _j.at("aq");
  }
  else if (type == "Spotlight") {
    m_type = Type::Spotlight;
    m_dir = glm::vec3(_j.at("direction")[0], _j.at("direction")[1],_j.at("direction")[2]);
    m_pos = glm::vec3(_j.at("position")[0], _j.at("position")[1],_j.at("position")[2]);
    m_id = glm::vec3(_j.at("id")[0],_j.at("id")[1],_j.at("id")[2]);
    m_is = glm::vec3(_j.at("is")[0],_j.at("is")[1],_j.at("is")[2]);
    m_ac = _j.at("ac");
    m_al = _j.at("al");
    m_aq = _j.at("aq");
    m_aa = _j.at("aa");
    m_cutoff = _j.at("cutoff");
  }
}

glm::vec3
Light::
processLight(Collision _c, BodyPtr _object, std::vector<BodyPtr> _objs) {
  glm::vec3 la, ld, ls, L;
  glm::vec3 l = glm::normalize(m_pos - _c.m_hit);
  Material m = _object->getMaterial();
  // ambient
  la = (m.m_ka * m_ia);
  // diffuse
  ld = (m.m_kd * m_id) * (float)(fmax(0, glm::dot(_c.m_normal,l)));
  // specular
  glm::vec3 h = glm::normalize(_c.m_v+l);
  ls = (m.m_ks * m_is) * powf(fmax(0, glm::dot(_c.m_normal, h)), m.m_n);

  //shadow: 
  bool isShadow = false;
    Ray shadowRay(_c.m_hit, l);
    for (size_t j=0; j<_objs.size(); ++j){
      Collision block = _objs[j]->intersects(shadowRay);
      if (block.isCollided){
        isShadow = true;
        break;
      }
    }

  // combine
  if (isShadow)
    L = la;
  else
    L = la + ld + ls;
  
  return L;
}

void
Light::
initialize(GLuint& _program, size_t _i) {
  //iaLoc = glGetUniformLocation(_program, "ia");

  // Request location
  using namespace std;
  // id
  string loc = "lights[" + to_string(_i) + "].id";
  idLoc = glGetUniformLocation(_program, loc.c_str());
  // is
  loc = "lights[" + to_string(_i) + "].is";
  isLoc = glGetUniformLocation(_program, loc.c_str());
  // lpos
  loc = "lights[" + to_string(_i) + "].pos";
  lPosLoc = glGetUniformLocation(_program, loc.c_str());
  // ac
  loc = "lights[" + to_string(_i) + "].ac";
  acLoc = glGetUniformLocation(_program, loc.c_str());
  // al
  loc = "lights[" + to_string(_i) + "].al";
  alLoc = glGetUniformLocation(_program, loc.c_str());
  // aq
  loc = "lights[" + to_string(_i) + "].aq";
  aqLoc = glGetUniformLocation(_program, loc.c_str());
  // aa
  loc = "lights[" + to_string(_i) + "].aa";
  aaLoc = glGetUniformLocation(_program, loc.c_str());
  // type
  loc = "lights[" + to_string(_i) + "].type";
  typeLoc = glGetUniformLocation(_program, loc.c_str());
  // cutoff
  loc = "lights[" + to_string(_i) + "].cutOff";
  cutoffLoc = glGetUniformLocation(_program, loc.c_str());
  // direction
  loc = "lights[" + to_string(_i) + "].dir";
  dirLoc = glGetUniformLocation(_program, loc.c_str());



  // sending uniforms
  //glUniform3fv(iaLoc, 1, glm::value_ptr(m_ia));
  glUniform3fv(idLoc, 1, glm::value_ptr(m_id));
  glUniform3fv(isLoc, 1, glm::value_ptr(m_is));
  glUniform3fv(lPosLoc, 1, glm::value_ptr(m_pos));
  glUniform3fv(dirLoc, 1, glm::value_ptr(m_dir));
  glUniform1f(acLoc, m_ac);
  glUniform1f(alLoc, m_al);
  glUniform1f(aqLoc, m_aq);
  glUniform1f(aaLoc, m_aa);
  glUniform1f(cutoffLoc, glm::cos(glm::radians(m_cutoff)));
  //glUniform1f(cutoffLoc, m_cutoff);
  glUniform1i(typeLoc, static_cast<int>(m_type));

  // std::cout << "Type: " << static_cast<int>(m_type)
  // << "\nCutoff : " << m_cutoff << std::endl;





}