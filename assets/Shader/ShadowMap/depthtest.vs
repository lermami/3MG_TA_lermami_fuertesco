#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec3 a_normal;


out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_m_matrix;
uniform mat4 u_v_matrix;
uniform mat4 u_p_matrix;


uniform mat4 u_light_space_matrix;

out vec2 TexCoords;

void main()
{
    vs_out.FragPos = vec3(u_m_matrix * vec4(vp, 1.0));
    vs_out.Normal = transpose(inverse(mat3(u_m_matrix))) * a_normal;
    vs_out.TexCoords = a_uv;
    vs_out.FragPosLightSpace = u_light_space_matrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vec4(vp, 1.0);

}