#version 460 core

out vec4 FragColor;

in vec2 TexCoords; 

uniform sampler2D hdrBuffer;
uniform sampler2D blurBuffer;
uniform bool uBlack;

void main() {

    float exposure = 2.2f;
    float gamma = 0.1f;

    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 blurColor = texture(blurBuffer, TexCoords).rgb;
    vec3 result;

    if (uBlack) {

        //hdrColor += blurColor;
        result = vec3(1.0f) - exp(-hdrColor * exposure);
        result = pow(result, vec3(1.0f / gamma));
        
        FragColor = vec4(hdrColor, 1.0f);
    } else {
        //result = hdrColor + blurColor;
        //result += pow(hdrColor, vec3(1.0f / gamma));
        FragColor = vec4(hdrColor, 1.0f);
    }

}