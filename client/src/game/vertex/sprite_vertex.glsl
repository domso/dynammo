#version 330

layout(location = 0) in vec3 vCoord;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 position;
layout(location = 3) in float typeIn;

// uniform vec3 position;
uniform vec2 scale;
uniform sampler2D mapData;
uniform vec2 camera;
uniform vec2 screenResolution;
uniform vec2 zoom;
out vec2 UV;

void main() {    
    float size = 0.005;
    float resolution = 512;
    float scale = 1;
    float depthMargin = 1.0f / (2.0f * resolution);
    
    vec3 scaledCoord = vCoord;
    scaledCoord.x *= zoom.x * size * 4 * 1.001;
    scaledCoord.y *= zoom.y * (screenResolution.x / screenResolution.y) * size * 4 * 1.001;
    

    
    vec3 correctedPosition = position;
    correctedPosition.x += 1;
    correctedPosition.y += 1;    
    
    float center = (texture(mapData, vec2(correctedPosition.x / resolution, correctedPosition.y / resolution)).x) * scale;    
    vec3 outputPosition;
    
    outputPosition.x = -((correctedPosition.x - correctedPosition.y) * 2 * size) * zoom.x;
    outputPosition.y = -((correctedPosition.x + correctedPosition.y) * size - center - position.z * size * 2) * zoom.y * (screenResolution.x / screenResolution.y);
    outputPosition.z = outputPosition.y * depthMargin - (depthMargin) * position.z + abs(vCoord.x) * size * depthMargin;//-((correctedPosition.x + correctedPosition.y) * size - center) * zoom.y * (screenResolution.x / screenResolution.y);
    
    
    
    scaledCoord.z *= 0;
    depthMargin * 2;
    
    
    gl_Position = vec4(scaledCoord + outputPosition - vec3(camera * zoom, 0), 1);    
    
    
    
    
    
    vec2 outputUV = vertexUV;
    
    outputUV.x /= 3;
    outputUV.y /= 3;

    int t = int(typeIn);
    outputUV.x += (t % 3) * (1f / 3);
    outputUV.y += (t / 3) * (1f / 3);
    
    
    
    UV = outputUV;
}

