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
uniform sampler2D depthMap;
uniform sampler2D emissionMap;
uniform float height_scale;
uniform float fogDensity;
uniform vec3 fogColor;

struct LightInfo {
  vec3 id, is;
  int  type;
  vec3 pos, dir;
  float ac, al, aq, aa, cutOff;  ///< cutoff is cos
};

const int size = 8;
uniform LightInfo lights[size];


vec3 computeLight(LightInfo _light, vec3 _normal, vec3 _view, vec2 _textCoord);
vec3 computeNewNormal(vec2 _textCoord);
vec2 computeTextureCoords(vec3 _viewDir);

void main() {
  // Ambient
  vec3 ambient = ia * ka;
  vec3 viewDir = normalize(vPos - fpos);
  vec2 ntextCoord = computeTextureCoords(viewDir);
  if(ntextCoord.x > 1.0 || ntextCoord.y > 1.0 || ntextCoord.x < 0.0 || ntextCoord.y < 0.0)
    discard;
  vec3 norm = computeNewNormal(ntextCoord);
  // norm = normalize(normal);
  

  // Result
  vec3 L = ambient;
  for(int i = 0; i<size; ++i){
    L += computeLight(lights[i], norm, viewDir, ntextCoord);
  } 
  // emission
  vec3 ke = vec3(texture(emissionMap, ntextCoord));
  L += ke;
  // fog 
  float dist = length(vPos - fpos);
  float f = exp(-fogDensity*dist);
  L = f * L + (1-f) * fogColor;
  // fcolor = f * vec4(L, 1.0) + (1-f) * vec4(fogColor,1.0);
  // float a = texture(samp1, textCoord)[3];
  fcolor = min(vec4(L, 1.0), vec4(1.0,1.0,1.0,1.0));
  // fcolor = (vec4(normalize(fpos), 1.0) + 1) / 2;
  // fcolor = texture(emissionMap, textCoord);
}

vec2 computeTextureCoords(vec3 _viewDir) {
  vec3 nnormal = normalize(normal);
  vec3 ntangent = normalize(tangent);
  ntangent = normalize(ntangent - dot(ntangent, nnormal) * nnormal);
  // TO DO check orientation
  vec3 bitangent = cross(ntangent, nnormal);
  mat3 tbn = transpose(mat3(ntangent, bitangent, nnormal));

  vec3 viewDir = normalize(tbn * _viewDir);

  // number of depth layers
  // const float numLayers = 10;
  const float minLayers = 8.0;
  const float maxLayers = 32.0;
  float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), _viewDir), 0.0)); 
  // calculate the size of each layer
  float layerDepth = 1.0 / numLayers;
  // depth of current layer
  float currentLayerDepth = 0.0;
  // the amount to shift the texture coordinates per layer (from vector P)
  vec2 P = viewDir.xy * height_scale; 
  vec2 deltaTexCoords = P / numLayers;

  // get initial values
  vec2  currentTexCoords     = textCoord;
  float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    
  while(currentLayerDepth < currentDepthMapValue)
  {
      // shift texture coordinates along direction of P
      currentTexCoords -= deltaTexCoords;
      // get depthmap value at current texture coordinates
      currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
      // get depth of next layer
      currentLayerDepth += layerDepth;  
  }

  // float height =  texture(depthMap, textCoord).r;    
  // vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
  // return textCoord - p; 
  return currentTexCoords;   

}

vec3 computeNewNormal(vec2 _textCoord) {
  vec3 nnormal = normalize(normal);
  vec3 ntangent = normalize(tangent);
  ntangent = normalize(ntangent - dot(ntangent, nnormal) * nnormal);  ///< ortho normalization
  // TO DO check orientation
  vec3 bitangent = cross(ntangent, nnormal);
  mat3 tbn = mat3(ntangent, bitangent, nnormal);
  vec3 retrievedNormal = texture(normMap, _textCoord).xyz;
  retrievedNormal = retrievedNormal * 2.0 - 1.0;
  vec3 newNormal = tbn * retrievedNormal;
  return normalize(newNormal);
}


vec3 computeLight(LightInfo _light, vec3 _normal, vec3 _view, vec2 _textCoord) {
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
  
  vec3 diffuse  = _light.id  * diff * vec3(texture(samp1, _textCoord));
  vec3 specular = _light.is * spec * vec3(texture(samp2, _textCoord));
  // vec3 diffuse  = _light.id  * diff * kd;
  // vec3 specular = _light.is * spec * ks;
  //ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (diffuse + specular);

}
