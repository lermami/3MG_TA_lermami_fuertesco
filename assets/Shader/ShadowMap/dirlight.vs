#version 330 core
layout(location = 0) in vec3 vp;

uniform mat4 u_light_space_matrix;
uniform mat4 u_m_matrix;

void main()
{
    gl_Position = u_light_space_matrix * u_m_matrix * vec4(vp, 1.0);
} 