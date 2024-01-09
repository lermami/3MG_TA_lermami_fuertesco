#version 330

out vec4 frag_colour;

in vec4 color;
in vec3 pos;
in vec2 uv;

uniform sampler2D u_texture;

void main() {
   frag_colour = texture(u_texture, uv);
};