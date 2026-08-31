#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <map>
#include <glm/glm.hpp>

class Shader {
    public:
        unsigned int ID;
        std::map<std::string, int> uniformLocations;
        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix;
        glm::mat4 modelViewMatrix;
        glm::mat3 normalMatrix;

        Shader(const char * vertexPath, const char * fragmentPath);

        void use();
        void addUniformLocation(std::string uniform, int location);

    private:
        void checkErrors(unsigned int shader, std::string type);    
};

#endif