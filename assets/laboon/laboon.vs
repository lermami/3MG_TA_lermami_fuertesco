#version 330


uniform mat4 u_m_matrix;
uniform mat4 u_v_matrix;

uniform mat4 u_o_matrix;
uniform mat4 u_p_matrix;

uniform mat4 u_light_space_matrix;

uniform vec3 u_camera_pos;


layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec3 a_normal;

out vec4 color;
out vec3 pos;
out vec2 uv;
out vec3 normal;
out vec3 frag_pos;
out vec4 frag_pos_light_space;

out vec3 world_position;
out vec3 world_normal;
out vec3 cam_dir;

void main() { 
  
  gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vec4(vp, 1.0);
  
  frag_pos = vec3(u_m_matrix * vec4(vp, 1.0));
  
  world_position = ((u_m_matrix * vec4(vp, 1.0f)).xyz);
  world_normal = normalize((u_m_matrix * vec4(a_normal, 0.0f)).xyz); //mat3(u_m_matrix) * a_normal;
  cam_dir = normalize(u_camera_pos - (u_m_matrix * vec4(world_position, 1.0f)).xyz);

  frag_pos_light_space = u_light_space_matrix * vec4(frag_pos, 1.0);
  
  color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  uv = a_uv;
  normal = a_normal;
  pos = vp;
};