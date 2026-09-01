#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform float uTime;
uniform float uStartTime;
uniform bool uBlack;
uniform vec3 uLightPos;

in vec3 Normal;
in vec3 FragPos;

const vec3 weight = vec3(0.2126, 0.7152, 0.0722);

void main() {

    vec3 objColor = vec3(0.85f, 0.9f, 0.53f); 
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = uLightPos.xyz;

    // AMBIENT -----------------------------------------------------
    float ambientStrength = 0.3f;

    vec3 ambient = ambientStrength * lightColor;

    // DIFFUSE -----------------------------------------------------
    vec3 norm = Normal;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0f);

    vec3 diffuse = diff * lightColor * 0.8f;

    // RIM LIGHT (FRESNEL) -----------------------------------------
    vec3 viewDir = normalize(-FragPos);
    vec3 rimColor = vec3(1.0f, 1.0f, 1.0f);
    float fresnel = 0.3f;
    float rimIntensity = pow(1.0f - dot(viewDir, norm), fresnel);

    rimColor *= rimIntensity;

    float progress = clamp((uTime - uStartTime) / 10.0f, 0.0f, 0.99f);
    
    if (!uBlack) {
        progress = 0.0f;
        rimColor = vec3(0.0f);
    } else {
        diffuse = vec3(0.0f);
    }
    
    vec3 result = objColor * (ambient + diffuse + rimColor);

    vec3 black = vec3(0.0f, 0.0f, 0.0f);
    float luminosity = dot(result, weight);
    
    //result = mix(result, black, progress);
    FragColor = vec4(result, 1.0f);
    
    if (luminosity > 0.75f) {
        BrightColor = FragColor;
    } else {
        BrightColor = vec4(black, 1.0f);
    }
}