#version 330 core
out vec4 FragColor;
  
in vec2 uv;

uniform sampler2D u_depth_map;

void main()
{             
    float depthValue = texture(u_depth_map, uv).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}  