#version 460 core

out vec4 FragColor;

uniform float uTime;
uniform float uStartTime;
uniform bool uBlack;

in vec3 Normal;
in vec3 FragPos;

void main() {

    vec3 objColor = vec3(1.0f, 0.2f, 0.4f); //coral-ish red
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.7f;
    vec3 lightPos = vec3(0.0f, 10.0f, 0.0f);

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = diff * lightColor * 0.8f;

    float progress = clamp((uTime - uStartTime) / 5.0f, 0.0f, 0.95f);
    vec3 result = objColor * (ambient + diffuse);

    if (!uBlack) {
        progress = 0.0f;
    }

    vec3 black = vec3(0.0f, 0.0f, 0.0f);
    result = mix(result, black, progress);

    FragColor = vec4(result, 1.0f);
}