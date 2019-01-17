#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform sampler2D mapData;
uniform int regionID;
uniform vec2 camera;
uniform vec2 screenResolution;
uniform vec2 zoom;

uniform float waterOffset;

out float height;
out vec3 normal;


// uniform sampler2D mapData;

void main() {
    float size = 0.005;
    float resolution = 512;
    float scale = 1;
    float depthMargin = 1.0f / (2.0f * resolution);
    vec3 outputPosition;
    
    float center = texture(mapData, vec2(position.x / resolution, position.y / resolution)).x * scale;
    float top = texture(mapData, vec2((position.x) / resolution, (position.y - 1) / resolution)).x * scale;
    float bot = texture(mapData, vec2((position.x) / resolution, (position.y + 1) / resolution)).x * scale;
    float left = texture(mapData, vec2((position.x - 1) / resolution, (position.y) / resolution)).x * scale;
    float right = texture(mapData, vec2((position.x + 1) / resolution, (position.y) / resolution)).x * scale;

    height = center;
    vec3 normalVector;
    normalVector.x = -(right - left);
    normalVector.y = -(bot - top);
    normalVector.z = 5f / resolution;

    
    
    vec3 scaledPosition = position;
    
//     scaledPosition.x -= (regionID >> 16) * 126;
//     scaledPosition.y -= ((regionID << 16) >> 16) * 126;
    
    float tmp = regionID;
    //
    outputPosition.x = -((scaledPosition.x - scaledPosition.y) * 2 * size) * zoom.x;
    outputPosition.y = -((scaledPosition.x + scaledPosition.y) * size - center) * zoom.y * (screenResolution.x / screenResolution.y);
    outputPosition.z = -((scaledPosition.x + scaledPosition.y + 1 + 0) * depthMargin);
            
    
//     vec3 scaleV = vec3(scale, scale, scale);
    
        
    
    gl_Position = vec4(outputPosition - vec3(camera * zoom, 0), 1);
    UV = vertexUV;
    normal = normalize(normalVector);
}

