#version 330

struct AmbientLight
{
  vec3 pos_;
  vec3 color_;
  vec3 spec_color_;
};

struct DirectionalLight
{
  vec3 pos_;
  vec3 color_;
  vec3 spec_color_;

  vec3 direction_;
};

struct PointLight
{
  vec3 pos_;
  vec3 color_;
  vec3 spec_color_;

  float constant_;
  float linear_;
  float quadratic_;
};

struct SpotLight
{
  vec3 pos_;
  vec3 color_;
  vec3 spec_color_;

  vec3 direction_;

  float constant_;
  float linear_;
  float quadratic_;

  float cutoff_angle_;
};

out vec4 frag_colour;

in vec4 color;
in vec3 pos;
in vec2 uv;
in vec3 normal;

in vec3 world_position;
in vec3 world_normal;
in vec3 cam_dir;

uniform sampler2D u_texture;

//Lights
uniform AmbientLight u_ambient_light[5];
uniform DirectionalLight u_directional_light[5];
uniform PointLight u_point_light[5];
uniform SpotLight u_spot_light[5];

vec3 CalculateAmbientLight(AmbientLight light){

  AmbientLight aux_light = light;
  
  vec3 result = aux_light.color_ + aux_light.spec_color_;
  result = max(result, 0.0);
  
  return result; 
}

vec3 CalculateDirectionalLight(DirectionalLight light){

  DirectionalLight aux_light = light;
  
  // color_
  vec3 lightDir = normalize(aux_light.direction_);
  float diff = max(dot(world_normal, lightDir), 0.0);
  aux_light.color_ *= diff;

  // spec_color_
  vec3 reflectDir = normalize(reflect(-lightDir, world_normal));
  float spec = pow(max(dot(cam_dir, reflectDir), 0.0), 0.5) * 0.5;//TODO: Fix Shyni
  aux_light.spec_color_ *= spec * 0.2;
  
  vec3 result = aux_light.spec_color_ + aux_light.color_;
  result = max(result, 0.0);

  return result;
}

vec3 CalculatePointLight(PointLight light){

  PointLight aux_light = light;

  // color_ 
  vec3 lightDir = aux_light.pos_ - world_position;
  float distance = length(lightDir);
  lightDir = normalize(lightDir);
  float attenuation = 1.0 / (aux_light.constant_ + aux_light.linear_ * distance + aux_light.quadratic_ * (distance * distance));    
  lightDir *= attenuation * 3.0;//TODO: Fix Intensity
  aux_light.color_ = max(dot(lightDir, world_normal), 0.0) * aux_light.color_;

  // spec_color_
  vec3 reflectDir = normalize(reflect(-lightDir, world_normal));
  float spec = pow(max(dot(cam_dir, reflectDir), 0.5), 32);//TODO: Fix Shyni
  aux_light.spec_color_ *= spec * 0.2 * attenuation;

  vec3 result = aux_light.color_ + aux_light.spec_color_;
  result = max(result, 0.0);

  return result; 
}

vec3 CalculateSpotLight(SpotLight light){

  SpotLight aux_light = light;

  // color_ 
  vec3 viewDir = aux_light.pos_ - world_position;
  
  float distance = length(viewDir);
  viewDir = normalize(viewDir);
  float attenuation = 1.0 / (aux_light.constant_ + aux_light.linear_ * distance + aux_light.quadratic_ * (distance * distance));    
  viewDir *= attenuation;
  aux_light.color_ = max(dot(viewDir, world_normal),0.0) * aux_light.color_;

  // spec_color_
  vec3 reflectDir = normalize(reflect(-viewDir, world_normal));
  float spec = pow(max(dot(cam_dir, reflectDir), 0.5), 32);//TODO: Fix Shyni
  aux_light.spec_color_ *= spec * 0.2 * attenuation;
  

  vec3 result = aux_light.color_ + aux_light.spec_color_;

  float angle = dot(aux_light.direction_, viewDir);
  result *= step(aux_light.cutoff_angle_, angle);

  return result; 
}


//kAmbient = 0,
//kDirectional = 1,
//kPoint = 2,
//kSpot = 3,



vec3 LightProcess(){
  vec3 result = vec3(0.0, 0.0, 0.0);
  
  for(int i=0; i<5; i++){
    result += max(CalculateAmbientLight(u_ambient_light[i]), 0.0);
  }
 
  for(int i=0; i<5; i++){
    result += max(CalculateDirectionalLight(u_directional_light[i]), 0.0);
  }
  
  for(int i=0; i<5; i++){
    result += max(CalculatePointLight(u_point_light[i]), 0.0);
  }
  
  for(int i=0; i<5; i++){
    result += max(CalculateSpotLight(u_spot_light[i]), 0.0);
  }

  return result;
}


void main() {  

  frag_colour = texture(u_texture, uv);


  vec3 light = LightProcess();
  frag_colour *= vec4(light, 1.0);
};