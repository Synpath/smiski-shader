#version 460 core

out vec4 FragColor;

in vec2 TexCoords; 
in vec2 Position;

uniform sampler2D hdrBuffer;
uniform sampler2D blurBuffer;
uniform bool uBlack;

float circleSDF(vec2 coords) {
    vec2 center = vec2(400, 500);
    float radius = 150;
    return distance(coords, center) - radius;
}

void main() {

    float exposure = 0.7f;
    float gamma = 1.6f;

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

    float dist = circleSDF(Position); // uv coords in screen space coords

    if (dist >= 0.0f) {
        //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}