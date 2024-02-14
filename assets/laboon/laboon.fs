#version 330

struct AmbientLight
{
  vec3 color_;
};

struct DirectionalLight
{
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
in vec3 frag_pos;
in vec4 frag_pos_light_space;

uniform sampler2D u_depth_map;
uniform sampler2D u_texture;

//Lights
uniform AmbientLight u_ambient_light[5];
uniform DirectionalLight u_directional_light[5];
uniform PointLight u_point_light[5];
uniform SpotLight u_spot_light[5];

vec3 CalculateAmbientLight(AmbientLight light){

  AmbientLight aux_light = light;
  
  vec3 result = aux_light.color_;
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

vec3 AmbientProcess(){
  vec3 result = vec3(0.0, 0.0, 0.0);

  for(int i=0; i<5; i++){
    result += max(CalculateAmbientLight(u_ambient_light[i]), 0.0);
  }

  return result;
}

float ShadowProcess(vec4 pos_light_space){

    // perform perspective divide
    vec3 projCoords = pos_light_space.xyz / pos_light_space.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_depth_map, projCoords.xy).r; 

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


void main() {

  vec3 light = LightProcess();
  vec3 ambient = AmbientProcess();

  float shadow = ShadowProcess(frag_pos_light_space);

  vec3 result = (ambient + (1.0 - shadow) * light) * texture(u_texture, uv).rgb;

  frag_colour = vec4(result, 1.0);
  //frag_colour = texture(u_texture, uv);

  //float depthValue = texture(u_depth_map, uv).r;
  //frag_colour = vec4(vec3(depthValue), 1.0); // orthographic
};