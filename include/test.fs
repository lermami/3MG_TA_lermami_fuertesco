#version 400

out vec4 frag_colour;
in vec3 color;
void main() {
    //frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
    frag_colour = vec4(color, 1.0);
};