#version 330

layout(location = 0) in vec3 vCoord;
layout(location = 1) in vec2 vertexUV;

uniform vec3 position;
uniform vec2 scale;
out vec2 UV;

void main() {    
    vec3 scaledCoord = vCoord;
    scaledCoord.x *= scale.x;
    scaledCoord.y *= scale.y;
    
    
    gl_Position = vec4(scaledCoord + position, 1);    
    UV = vertexUV;
}

