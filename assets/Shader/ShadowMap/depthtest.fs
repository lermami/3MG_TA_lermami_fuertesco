#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_depth_map;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 0.01 * 100000.0) / (100000.0 + 0.01 - z * (100000.0 - 0.01));	
}

void main()
{             
    float depthValue = texture(u_depth_map, TexCoords).r;
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / 100000), 1.0); // perspective
   FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}