#version 330

out vec4 frag_colour;

in vec3 pos;
in vec2 uv;
in vec3 normal;

in vec3 world_position;
in vec3 world_normal;
in vec3 cam_dir;

uniform sampler2D u_texture;


void main() {

  frag_colour = texture(u_texture, uv);
};