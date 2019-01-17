#version 330

out vec4 outputColor;
in float height;
in vec2 UV;
uniform sampler2D groundTex;
uniform sampler2D groundTex2;
uniform sampler2D groundTex3;
uniform sampler2D groundTex4;
in vec3 normal;

void main() {
     vec4 point = texture(groundTex, UV);
     vec4 point2 = texture(groundTex2, UV);
     vec4 point3 = texture(groundTex3, UV);
     vec4 point4 = texture(groundTex4, UV);
     
    
    
    
    float cosTheta = dot(normal,vec3(0, 0, 1));    
    point = mix(point, point2, cosTheta);  
    point3 = mix(point3, point4, cosTheta);  
    
    
    
    point = mix(point, point3, min(1, height * 1.5));
    
    
    outputColor = point * cosTheta;
    outputColor.a = 1;

    //outputColor = p * cosTheta * min(height * 2, 1);//mix(p, vec4(height, height, height, 1), 0.2);
    
//     outputColor = mix(vec4(1.0f, 0.85f, 0.35f, 1.0f), 
//     vec4(0.2f, 0.4f, 0.4f, 1.0f), lerpVal);
}
