#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uTexture;
uniform bool uHorizontal;

uniform float weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main() {
    
    vec2 offset = 1.0f / textureSize(uTexture, 0);
    vec3 result = texture(uTexture, TexCoords).rgb * weights[0];

    if (uHorizontal) {
        for (int i = 1; i < 5; i++) {
            result += texture(uTexture, TexCoords + vec2(offset.x * i, 0.0f)).rgb * weights[i];
            result += texture(uTexture, TexCoords - vec2(offset.x * i, 0.0f)).rgb * weights[i];
        }
    } else {
        for (int i = 1; i < 5; i++) {
            result += texture(uTexture, TexCoords + vec2(0.0f, offset.y * i)).rgb * weights[i];
            result += texture(uTexture, TexCoords - vec2(0.0f, offset.y * i)).rgb * weights[i];
        }
    }

    //result = vec3(1.0f, 0.0f, 0.0f);
    FragColor = vec4(result, 1.0f);
}