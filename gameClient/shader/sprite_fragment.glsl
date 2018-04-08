#version 330

out vec4 outputColor;
in vec2 UV;
uniform sampler2D tex;

void main() {
    vec4 pixelColor = texture(tex, UV );   
    
    if (pixelColor.a < 0.5) {
        discard;
    }
    
    outputColor = pixelColor;
}

