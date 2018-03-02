#version 330

out vec4 outputColor;
in vec2 UV;
uniform sampler2D tex;

void main() {
    vec4 pixelColor = texture(tex, UV );    
    outputColor = pixelColor;
}

