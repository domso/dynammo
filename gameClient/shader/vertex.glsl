#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
uniform sampler2D mapData;
out float height;
out vec3 normal;

// uniform sampler2D mapData;

void main() {
    float size = 0.005;
    float resolution = 128;
    float scale = 2;
    float depthMargin = 1.0f / (2.0f * resolution);
    vec3 outputPosition;
                
    float center = texture(mapData, vec2(position.x / resolution, position.y / resolution)).x * scale;
    float top = texture(mapData, vec2((position.x) / resolution, (position.y - 1) / resolution)).x * scale;
    float bot = texture(mapData, vec2((position.x) / resolution, (position.y + 1) / resolution)).x * scale;
    float left = texture(mapData, vec2((position.x - 1) / resolution, (position.y) / resolution)).x * scale;
    float right = texture(mapData, vec2((position.x + 1) / resolution, (position.y) / resolution)).x * scale;

    vec3 normalVector;
    normalVector.x = -(right - left);
    normalVector.y = -(bot - top);
    normalVector.z = 2f / resolution;

    
    
    outputPosition.x = -((position.x - position.y) * 2 * size);
    outputPosition.y = -((position.x + position.y) * size - center);
    outputPosition.z = -((position.x + position.y) * depthMargin);
            
    
//     vec3 scaleV = vec3(scale, scale, scale);
    
    gl_Position = vec4(outputPosition, 1);
    UV = vertexUV;
    normal = normalize(normalVector);
}
