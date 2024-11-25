#include "Material.h"
#include "../GLInclude.h"
// SOIL2
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <iostream>
#include <sstream>

Material::
Material(json& _j) {
  m_ka = glm::vec3(_j.at("ka")[0],_j.at("ka")[1],_j.at("ka")[2]);
  m_kd = glm::vec3(_j.at("kd")[0],_j.at("kd")[1],_j.at("kd")[2]);
  m_ks = glm::vec3(_j.at("ks")[0],_j.at("ks")[1],_j.at("ks")[2]);
  m_n  = _j.at("n");
  m_reflect = _j.at("reflect");
  mapKd = _j.at("mapKd");
  mapKs = _j.at("mapKs");
  mapNM = _j.at("mapNM");
  mapDM = _j.at("mapDM");
  mapEM = _j.at("mapEM");
}

Material::
Material(const std::string& _filename, const std::string& _mName) {
  std::ifstream ifs(_filename);
  if(!ifs) {
    std::cout << "Failed to open file" << std::endl;
  }
  std::string line;
  while(getline(ifs, line)) {
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    if(tag == "Ns") {
      iss >> m_n;
    }
    else if(tag == "Ka") {
      iss >> m_ka.x >> m_ka.y >> m_ka.z;
    }
    else if(tag == "Kd") {
      iss >> m_kd.x >> m_kd.y >> m_kd.z;
    }
    else if (tag == "Ks")
      iss >> m_ks.x >> m_ks.y >> m_ks.z;
    else if (tag == "map_Kd"){
      iss >> mapKd;
    }
    else if (tag == "map_Ks")
      iss >> mapKs;
    else if (tag == "map_NM")
      iss >> mapNM;
    else if (tag == "map_DM")
      iss >> mapDM;
    else if (tag == "map_EM")
      iss >> mapEM;
  }
}

void
Material::
initialize(GLuint& _program) {
  m_texKd = SOIL_load_OGL_texture
	(
		mapKd.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
  m_texKs = SOIL_load_OGL_texture
	(
	  mapKs.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
  m_normMap = SOIL_load_OGL_texture
	(
		mapNM.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
  m_depthMap = SOIL_load_OGL_texture
	(
		mapDM.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
  m_emissionMap = SOIL_load_OGL_texture
	(
		mapEM.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

  kaLoc = glGetUniformLocation(_program, "ka");
  kdLoc = glGetUniformLocation(_program, "kd");
  ksLoc = glGetUniformLocation(_program, "ks");
  nLoc = glGetUniformLocation(_program, "n");
  texKdLoc = glGetUniformLocation(_program, "samp1");
  texKsLoc = glGetUniformLocation(_program, "samp2");
  texNMLoc = glGetUniformLocation(_program, "normMap");
  texDMLoc = glGetUniformLocation(_program, "depthMap");
  texEMLoc = glGetUniformLocation(_program, "emissionMap");
  dScaleLoc = glGetUniformLocation(_program, "height_scale");

}

void
Material::
sendUniform() {
  glActiveTexture(GL_TEXTURE0); 
  glBindTexture(GL_TEXTURE_2D, m_texKd);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texKs);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_normMap);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_depthMap);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, m_emissionMap);

  glUniform3fv(kaLoc, 1, glm::value_ptr(m_ka));
  glUniform3fv(kdLoc, 1, glm::value_ptr(m_kd));
  glUniform3fv(ksLoc, 1, glm::value_ptr(m_ks));
  glUniform1f(nLoc, m_n);
  glUniform1f(dScaleLoc, m_depthScale);
  glUniform1i(texKdLoc, 0);
  glUniform1i(texKsLoc, 1);
  glUniform1i(texNMLoc, 2);
  glUniform1i(texDMLoc, 3);
  glUniform1i(texEMLoc, 4);
}