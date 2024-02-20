#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 a_uv;


uniform mat4 u_m_matrix;
uniform mat4 u_v_matrix;

uniform mat4 u_o_matrix;
uniform mat4 u_p_matrix;

out vec2 TexCoords;

void main()
{
    TexCoords = a_uv;
    gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vec4(vp, 1.0);
}