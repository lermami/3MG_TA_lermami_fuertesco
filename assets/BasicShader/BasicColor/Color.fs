#version 330

in vec4 color;
in vec3 pos;
in vec2 uv;
in vec3 normal;

in vec3 world_position;
in vec3 world_normal;
in vec3 cam_dir;

uniform vec4 u_color;

out vec4 frag_colour;


void main() {

  frag_colour = u_color;
};