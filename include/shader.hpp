#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <map>

class Shader {
    public:
        unsigned int ID;
        std::map<std::string, int> uniformLocations;
        Shader(const char * vertexPath, const char * fragmentPath);

        void use();
        void addUniformLocation(std::string uniform, int location);

    private:
        void checkErrors(unsigned int shader, std::string type);    
};

#endif