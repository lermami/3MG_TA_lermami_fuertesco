#version 330

uniform mat4 u_m_matrix;

layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;

out vec4 color;
out vec3 pos;
out vec2 uv;
/*
mat4 ViewMatrix(){

    vec3 forward = normalize(vp - vec3(0,0,0));
    vec3 left = normalize(cross(vec3(0,0,1), vec3(0,1,0));
    vec3 updated_up = normalize(cross(vec3(-1,0,0), vec3(0,0,1)));

    float dot_left_pos = dot(left, vec3(0,0,0));
    float dot_up_pos = dot(updated_up, vec3(0,0,0)));
    float dot_fwd_pos = dot(forward, vec3(0,0,0)));

    mat4 mat = mat4(left.x, updated_up.x, -forward.x, 0,
                left.y, updated_up.y, -forward.y, 0,
                left.z, updated_up.z, -forward.z, 0,
                -dot_left_pos, -dot_up_pos, dot_fwd_pos, 1);

    return mat;
}
*/
void main() {
    gl_Position = u_m_matrix * vec4(vp, 1.0);
    color = a_color;
    pos = vp;
    uv = a_uv;
};