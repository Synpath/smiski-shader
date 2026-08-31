#include <glad/glad.h>
#include <object.hpp>

Object::Object(float * vData) {
    vertices = vData;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}