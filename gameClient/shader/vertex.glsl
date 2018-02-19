#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in float data;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 normal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

out float height;
out vec3 n;

void main() {
    height = data;
//     vec3 scaleV = vec3(scale, scale, scale);
    gl_Position = vec4(position, 1);
    
    UV = vertexUV;
    n = normal;
}
