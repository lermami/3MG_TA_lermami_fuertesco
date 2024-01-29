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
in Light light;

uniform sampler2D u_texture;



void main() {
   frag_colour = texture(u_texture, uv);
};