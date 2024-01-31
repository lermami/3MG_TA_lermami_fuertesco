#version 330

struct Light
{
  int type_;

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
uniform Light u_light[2];

vec3 DirectionalLight(Light light){

  Light aux_light = light;

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

vec3 PointLight(Light light){

  Light aux_light = light;
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

vec3 SpotLight(Light light){

  Light aux_light = light;
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

/*
kAmbient = 0,
kDirectional = 1,
kPoint = 2,
kSpot = 3,
*/


vec3 LightProcess(Light light){
  vec3 result;


  if(light.type_ == 0){
    result = light.color_;
  }

  if(light.type_ == 1){
    result = DirectionalLight(light);
  }

  if(light.type_ == 2){
    result = PointLight(light);
  }

  if(light.type_ == 3){
    result = SpotLight(light);
  }

  return result;
}


void main() {

  vec3 light = vec3(0.0, 0.0, 0.0);
  for(int i=0; i<2; i++){
    light += LightProcess(u_light[i]);
  }


   frag_colour = texture(u_texture, uv);
   frag_colour *= vec4(light, 1.0);
};