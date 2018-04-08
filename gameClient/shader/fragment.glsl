#version 330

out vec4 outputColor;
in float height;
in vec2 UV;
uniform sampler2D groundTex;
in vec3 normal;

void main() {
    



    vec4 point = texture(groundTex, UV);
    
    float cosTheta = dot(normal,vec3(0, 0, 1));    
    
    outputColor = point * cosTheta;
    outputColor.a = 1;
    
    //outputColor = p * cosTheta * min(height * 2, 1);//mix(p, vec4(height, height, height, 1), 0.2);
    
//     outputColor = mix(vec4(1.0f, 0.85f, 0.35f, 1.0f), 
//     vec4(0.2f, 0.4f, 0.4f, 1.0f), lerpVal);
}
