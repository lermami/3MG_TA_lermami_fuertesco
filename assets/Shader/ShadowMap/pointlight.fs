#version 330 core
in vec4 FragPos;

uniform vec3 u_light_pos;
uniform float u_far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - u_light_pos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}