#version 330

out vec4 outputColor;
in float height;
in vec2 UV;
uniform sampler2D tex;
in vec3 n;

void main() {
    float lerpVal = gl_FragCoord.y / 500.0f;
    
    vec4 c = vec4(UV.x, 0, UV.y, 1); 
    
    vec4 t = vec4(0, 0, 0, 1);
    
    vec4 p = texture( tex, UV );
    
    
    float g = p.x ;
    
    float cosTheta = dot(n,vec3(0, 0, 1));
    
    
    
    outputColor = p * cosTheta * min(height * 2, 1);//mix(p, vec4(height, height, height, 1), 0.2);
    
    mix(vec4(1.0f, 0.85f, 0.35f, 1.0f), 
    vec4(0.2f, 0.4f, 0.4f, 1.0f), lerpVal);
}
