#version 460 core

out vec4 FragColor;

uniform float uTime;
uniform vec3 uStartColor;
uniform vec3 uTargetColor;
uniform float uStartTime;

void main() {
    vec3 red = vec3(1.0f, 0.2f, 0.4f);
    vec3 black = vec3(0.0f, 0.0f, 0.0f);

    float progress = clamp((uTime - uStartTime) / 5.0f, 0.0f, 1.0f);

    FragColor = vec4(mix(uStartColor, uTargetColor, progress), 1.0f);

}