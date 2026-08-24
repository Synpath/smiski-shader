#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 Normal;

void main() {
    mat4 project = projection;
    vec4 pos = vec4(aPos, 1.0f);

    gl_Position = projection * view * model * pos;
    FragPos = vec3(model * pos);
    Normal = aNormal;
}