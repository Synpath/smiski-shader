#version 460 core

out vec4 FragColor;

in vec2 TexCoords; 

uniform sampler2D hdrBuffer;
uniform sampler2D blurBuffer;
uniform bool uBlack;

void main() {

    float exposure = 0.9f;
    float gamma = 2.2f;

    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 blurColor = texture(blurBuffer, TexCoords).rgb;
    vec3 result;

    if (uBlack) {

        vec3 color = hdrColor + blurColor;
        result = vec3(1.0f) - exp(-color * exposure);
        result = pow(result, vec3(1.0f / gamma));
        result *= vec3(0.85f, 0.9f, 0.53f);
        FragColor = vec4(result, 1.0f);
    } else {
        //result = hdrColor + blurColor;
        //result += pow(hdrColor, vec3(1.0f / gamma));
        FragColor = vec4(hdrColor, 1.0f);
    }

}