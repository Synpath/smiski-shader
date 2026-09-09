#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords; 

out vec2 TexCoords;
out vec2 Position;

void main() {
    TexCoords = aTexCoords;
    Position = TexCoords * vec2(1000, 800);

    gl_Position = vec4(aPos, 1.0f);
}