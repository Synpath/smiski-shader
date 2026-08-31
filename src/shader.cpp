#include "shader.hpp"
// #include  <filesystem>

Shader::Shader(const char * vertexPath, const char * fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);

        /*std::cout << "Working directory: "
          << std::filesystem::current_path()
          << '\n';*/

        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream;
        std::stringstream fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ: " << e.what() << std::endl;
        vertexFile.close();
        fragmentFile.close();
        return;
    }

    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    
    unsigned int vertex, fragment;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkErrors(ID, "PROGRAM");

    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::addUniformLocation(std::string uniform, int location) {
    uniformLocations.insert({uniform, location});
}

void Shader::checkErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];

    if (type == "VERTEX" || type == "FRAGMENT") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION ERROR: " << type << "\n" << infoLog << "\n" << std::endl;
        }
    } else if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINKING ERROR: " << infoLog << "\n" << std::endl;
        }
    } else {
        std::cout << "Unknown type inputted when checking for errors\n" << std::endl;
    }
}