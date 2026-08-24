#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "iostream"
#include "shader.hpp"

// g++ -o main src/main.cpp src/glad.c -I./include -lGL -lglfw
// g++ -o main src/main.cpp src/glad.c src/shader.cpp -I./include -lGL -lglfw

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

static bool black; //tracks whether background is black atm
const int WIDTH = 1000;
const int HEIGHT = 800;

float startTime = 0.0f;
const float duration = 5.0f;
const glm::vec4 lightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);

int main () {

    // SETUP --------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "Glow in the Dark Shader", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failure to initialize window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    black = 0;
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);
    //-----------------------------------------------------------------

    // OBJECTS/SHADERS ------------------------------------------------

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

    // set up buffers
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // set up shaders
    const char * vert = "src/shaders/smiski.vert";
    const char * frag = "src/shaders/smiski.frag";
    Shader smiski(vert, frag);
    smiski.use();

    smiski.addUniformLocation("uTime", glGetUniformLocation(smiski.ID, "uTime"));
    smiski.addUniformLocation("uStartTime", glGetUniformLocation(smiski.ID, "uStartTime"));
    smiski.addUniformLocation("projection", glGetUniformLocation(smiski.ID, "projection"));
    smiski.addUniformLocation("model", glGetUniformLocation(smiski.ID, "model"));
    smiski.addUniformLocation("view", glGetUniformLocation(smiski.ID, "view"));
    smiski.addUniformLocation("uBlack", glGetUniformLocation(smiski.ID, "uBlack"));
    smiski.addUniformLocation("modelView", glGetUniformLocation(smiski.ID, "modelView"));
    smiski.addUniformLocation("normalMat", glGetUniformLocation(smiski.ID, "normalMat"));
    smiski.addUniformLocation("uLightPos", glGetUniformLocation(smiski.ID, "uLightPos"));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // MATRICES ----------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv(smiski.uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(36.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(smiski.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    glUniformMatrix4fv(smiski.uniformLocations["view"], 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 modelView = view * model;

    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));
    glUniformMatrix3fv(smiski.uniformLocations["normalMat"], 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glm::vec4 lightpos = view * lightPos;
    glUniform4fv(smiski.uniformLocations["uLightPos"], 1, glm::value_ptr(lightPos));

    // RENDER LOOP ----------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        glfwPollEvents();
        
        float time = glfwGetTime();

        glUniform1f(smiski.uniformLocations["uTime"], time);
        glUniform1f(smiski.uniformLocations["uStartTime"], startTime);
        glUniform1i(smiski.uniformLocations["uBlack"], black);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }
    
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(smiski.ID);
    glfwTerminate();
    return 0;
} //

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

        float now = glfwGetTime();

        if (black) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        } else {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        
        startTime = now;
        black = !black;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
