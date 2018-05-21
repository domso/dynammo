#version 330

layout(location = 0) in vec3 vCoord;
layout(location = 1) in vec2 vertexUV;

uniform vec3 position;
uniform vec2 scale;
uniform sampler2D mapData;
uniform vec2 camera;
uniform vec2 screenResolution;
uniform vec2 zoom;

uniform int frameDimension;
uniform int frame;

out vec2 UV;

void main() {    
    vec3 scaledCoord = vCoord;
    scaledCoord.x *= scale.x * zoom.x;
    scaledCoord.y *= scale.y * zoom.y * (screenResolution.x / screenResolution.y);
    
    float size = 0.005;
    float resolution = 128;
    float scale = 2;
    float depthMargin = 1.0f / (2.0f * resolution);
    
    float center = texture(mapData, vec2(position.x / resolution, position.y / resolution)).x * scale;
    
    vec3 outputPosition;
    
    outputPosition.x = -((position.x - position.y) * 2 * size) * zoom.x;
    outputPosition.y = -((position.x + position.y) * size - center) * zoom.y * (screenResolution.x / screenResolution.y);
    outputPosition.z = -((position.x + position.y) * depthMargin);
    
    
    gl_Position = vec4(scaledCoord + outputPosition - vec3(camera, 0), 1);  
    
    
    vec2 frameUV = vertexUV;
    
    frameUV.x /= frameDimension;
    frameUV.y /= frameDimension;
    
    frameUV.x += (frame % frameDimension) * (1f / frameDimension);
    frameUV.y += (frame / frameDimension) * (1f / frameDimension);
    
    UV = frameUV;
}

