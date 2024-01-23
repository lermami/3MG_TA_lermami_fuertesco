#version 330

uniform mat4 u_m_matrix;
uniform mat4 u_v_matrix;
uniform mat4 u_o_matrix;
uniform mat4 u_p_matrix;
uniform vec3 u_camera_pos;


layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec3 a_normal;

out vec4 color;
out vec3 pos;
out vec2 uv;
out vec3 normal;

out vec3 world_position;
out vec3 world_normal;
out vec3 cam_dir;

mat4 ViewMat(){
  vec3 forward = normalize(vec3(0, 0, -2) - u_camera_pos);
  vec3 right = normalize(cross(vec3(0,1,0), forward));
  vec3 updated_up = normalize(cross(forward, right));

  float dot_right_pos = dot(right, u_camera_pos);
  float dot_up_pos = dot(updated_up, u_camera_pos);
  float dot_fwd_pos = dot(forward, u_camera_pos);

  mat4 m = mat4(right.x,         updated_up.x,   -forward.x,   0.0f,
                right.y,         updated_up.y,   -forward.y,   0.0f,
                right.z,         updated_up.z,   -forward.z,   0.0f,
                -dot_right_pos,  -dot_up_pos,    dot_fwd_pos,  1.0f);
  
  return m;
}


mat4 OrthographicMat(float left, float right, float top, float bottom, float near, float far){

  mat4 m = mat4(2.0f/(right-left),          0.0f,                       0.0f,                    0.0f,
                0.0f,                       2.0f/(top-bottom),          0.0f,                    0.0f,
                0.0f,                       0.0f,                       -2.0f/(far-near),        0.0f,
                -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near),  1.0f);

    return m;
}

mat4 PerpectiveMat(float fov, float aspect, float near, float far){

  mat4 m = mat4(1.0f/aspect*fov,    0.0f,         0.0f,                           0.0f,
                0.0f,               1.0f/fov,     0.0f,                           0.0f,
                0.0f,               0.0f,         (-far-near)/(far-near),         -1.0f,
                0.0f,               0.0f,         -(2.0f*far*near)/(far-near),    0.0f);

  return m;
}

void main() { 
  
  mat4 v_matrix = ViewMat();
  mat4 p_matrix = OrthographicMat(-1.0f, 1.0f, 1.0f, -1.0f, 0.01f, 100.0f);
  //mat4 p_matrix = PerpectiveMat(60.0f, 1024.0f/768.0f, 0.1f, 100.0f);

  gl_Position = u_o_matrix * u_v_matrix * u_m_matrix * vec4(vp, 1.0);
  
	world_position = vp;                          //((u_m_matrix * vec4(a_position, 1.0f)).xyz);
	world_normal = mat3(u_m_matrix) * a_normal;   //normalize((u_m_matrix * vec4(a_normal, 0.0f)).xyz);
	cam_dir = normalize(u_camera_pos - (u_m_matrix * vec4(world_position, 1.0f)).xyz);
  
  color = a_color;
	uv = a_uv;
	normal = mat3(u_m_matrix) * a_normal;
	pos = vp;
};