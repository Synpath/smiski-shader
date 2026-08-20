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
glm::vec3 startColor = glm::vec3(1.0f, 0.2f, 0.4f);
glm::vec3 targetColor = glm::vec3(1.0f, 0.2f, 0.4f);

int main () {

    // WINDOW SETUP --------------------------------------------------------------
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
    //-----------------------------------------------------------------

    // OBJECTS/SHADERS ------------------------------------------------

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // set up shaders
    const char * vert = "src/shaders/smiski.vert";
    const char * frag = "src/shaders/smiski.frag";
    Shader smiski(vert, frag);
    smiski.use();

    smiski.addUniformLocation("uTime", glGetUniformLocation(smiski.ID, "uTime"));
    smiski.addUniformLocation("uStartColor", glGetUniformLocation(smiski.ID, "uStartColor"));
    smiski.addUniformLocation("uTargetColor", glGetUniformLocation(smiski.ID, "uTargetColor"));
    smiski.addUniformLocation("uStartTime", glGetUniformLocation(smiski.ID, "uStartTime"));
    smiski.addUniformLocation("projection", glGetUniformLocation(smiski.ID, "projection"));
    smiski.addUniformLocation("model", glGetUniformLocation(smiski.ID, "model"));
    smiski.addUniformLocation("view", glGetUniformLocation(smiski.ID, "view"));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // MATRICES ----------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv(smiski.uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(36.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(smiski.uniformLocations["model"], 1, GL_FALSE, &model[0][0]);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
    glUniformMatrix4fv(smiski.uniformLocations["view"], 1, GL_FALSE, glm::value_ptr(view));

    // RENDER LOOP ----------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);
        glfwPollEvents();
        
        float time = glfwGetTime();

        glUniform3fv(smiski.uniformLocations["uStartColor"], 1, glm::value_ptr(startColor));
        glUniform3fv(smiski.uniformLocations["uTargetColor"], 1, glm::value_ptr(targetColor));
        glUniform1f(smiski.uniformLocations["uTime"], time);
        glUniform1f(smiski.uniformLocations["uStartTime"], startTime);
        
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

        float progress = glm::clamp(
            (now - startTime) / duration, 0.0f, 1.0f
        );

        startColor = glm::mix(startColor, targetColor, progress);

        if (black) {
            startColor = glm::vec3(1.0f, 0.2f, 0.4f);
            targetColor = glm::vec3(1.0f, 0.2f, 0.4f);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        } else {
            targetColor = glm::vec3(0.0f);
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
