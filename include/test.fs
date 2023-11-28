#version 400

out vec4 frag_colour;

in vec4 color;
in vec3 pos;

void main() {
   //frag_colour = color;
   frag_colour = vec4(cos(pos), 1.0);
};