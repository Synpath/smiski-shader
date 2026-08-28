#version 460 core

out vec4 FragColor;

in vec2 TexCoords; 

uniform sampler2D hdrBuffer;
uniform bool uBlack;

void main() {

    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    if (uBlack) {
        FragColor = vec4(hdrColor, 1.0f);
    } else {
        FragColor = vec4(hdrColor, 1.0f);
    }

}