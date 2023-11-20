#version 400

uniform mat4 u_m_matrix;

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 a_color;
out vec3 color;
void main() {
    gl_Position = u_m_matrix * vec4(vp, 1.0);
    color = a_color;
};