////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data.
////////////////////////////////////////////////////////////////////////////////

#version 330

in  vec3  fpos;
in  vec3  normal; // Interpolated color from rasterization step
in  vec2  textCoord;
in  vec3  tangent;

out vec4 fcolor; // Output fragment color

uniform vec3 ka, kd, ks, vPos;
uniform float n;
uniform vec3 ia;
uniform mat4 v_matrix;
uniform sampler2D samp1;  // diffuse mapping
uniform sampler2D samp2;  // specular mapping
uniform sampler2D normMap;


struct LightInfo {
  vec3 id, is;
  int  type;
  vec3 pos, dir;
  float ac, al, aq, aa, cutOff;  ///< cutoff is cos
};

const int size = 8;
uniform LightInfo lights[size];


vec3 computeLight(LightInfo _light, vec3 _normal, vec3 _view);
vec3 computeNewNormal();

void main() {
  // Ambient
  vec3 ambient = ia * ka;

  vec3 norm = computeNewNormal();
  vec3 viewDir = normalize(vPos - fpos);

  // Result
  vec3 L = ambient;
  for(int i = 0; i<size; ++i){
    L += computeLight(lights[i], norm, viewDir);
  } 
  float a = texture(samp1, textCoord)[3];
  fcolor = min(vec4(L, a), vec4(1.0,1.0,1.0,1.0));

}

vec3 computeNewNormal() {
  vec3 nnormal = normalize(normal);
  vec3 ntangent = normalize(tangent);
  ntangent = normalize(ntangent - dot(ntangent, nnormal) * nnormal);
  // TO DO check orientation
  vec3 bitangent = cross(ntangent, nnormal);
  mat3 tbn = mat3(ntangent, bitangent, nnormal);
  vec3 retrievedNormal = texture(normMap, textCoord).xyz;
  retrievedNormal = retrievedNormal * 2.0 - 1.0;
  vec3 newNormal = tbn * retrievedNormal;
  return normalize(newNormal);
}


vec3 computeLight(LightInfo _light, vec3 _normal, vec3 _view) {
  vec3 lPos = (v_matrix * vec4(_light.pos, 1.0)).xyz;
  // directional
  vec3 lightDir;
  if (_light.type == 0){
    lightDir = normalize((v_matrix * vec4(-_light.dir, 0.0)).xyz);
  }
  else {
    lightDir = normalize(lPos - fpos);
  }
  // diffuse shading
  float diff = max(dot(_normal, lightDir), 0.0);
  // specular shading
  float spec;
  vec3 reflectDir = reflect(-lightDir, _normal);
  spec = pow(max(dot(_view, reflectDir), 0.0), n);
  // attenuation
  float attenuation = 1.f;
  float dist = length(lPos - fpos);

  // point and spotlight
  if (_light.type != 0){
    float theta = dot(lightDir, normalize((v_matrix * vec4(-_light.dir, 0.0)).xyz));
    if(theta > _light.cutOff) {       
      // do lighting calculations
      float linearAttenuation = 1.0 / (_light.ac + _light.al * dist + 
            _light.aq * (dist * dist));
      float angularAttenuation =  pow(theta, _light.aa);
      attenuation = linearAttenuation * angularAttenuation;  
    }
    else attenuation = 0;
  } 
  // combine results
  
  vec3 diffuse  = _light.id  * diff * vec3(texture(samp1, textCoord));
  vec3 specular = _light.is * spec * vec3(texture(samp2, textCoord));
  // vec3 diffuse  = _light.id  * diff * kd;
  // vec3 specular = _light.is * spec * ks;
  //ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (diffuse + specular);

}
