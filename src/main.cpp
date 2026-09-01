#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "iostream"
#include "shader.hpp"
#include "object.hpp"

// g++ -o main src/main.cpp src/glad.c -I./include -lGL -lglfw
// g++ -o main src/main.cpp src/glad.c src/shader.cpp -I./include -lGL -lglfw

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
GLFWwindow * initWindow();

static bool black; //tracks whether background is black atm
int WIDTH = 1000;
int HEIGHT = 800;

float startTime = 0.0f;
const float duration = 5.0f;
const glm::vec4 lightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);

int main () {

    // SETUP --------------------------------------------------------------
    GLFWwindow * window = initWindow();

    if (window == NULL) {
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    black = 1;
    glViewport(0, 0, WIDTH, HEIGHT);
    black ? glClearColor(0.0f, 0.0f, 0.0f, 1.0f) : glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);
    //-----------------------------------------------------------------

    // OBJECTS------------------------------------------------

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

    Object cube(vertices); 
    glBindVertexArray(cube.VAO);   
    glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    Object quad(quadVertices);

    glBindVertexArray(quad.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quad.VBO);
            
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);        
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set up buffers
    unsigned int hdrFBO, rboDepth;
    unsigned int colorBuffer[2];

    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    glGenTextures(2, colorBuffer);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, colorBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffer[i], 0);
    }
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete" << std::endl;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST); 
    
    // double buffers for blurring
    unsigned int blurFBO[2];
    unsigned int blurColor[2];
    glGenFramebuffers(2, blurFBO);
    glGenTextures(2, blurColor);
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
        glBindTexture(GL_TEXTURE_2D, blurColor[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColor[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer not complete" << std::endl;
        }
    }

    // SHADERS -----------------------------------------------
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

    Shader hdr("src/shaders/hdr.vert", "src/shaders/hdr.frag");
    hdr.use();
    hdr.addUniformLocation("uBlack", glGetUniformLocation(hdr.ID, "uBlack"));

    Shader blur("src/shaders/blur.vert", "src/shaders/blur.frag");
    blur.use();
    blur.addUniformLocation("uHorizontal", glGetUniformLocation(blur.ID, "uHorizontal"));

    // MATRICES ----------------------------------------------------
    smiski.use();

    smiski.modelMatrix = glm::rotate(smiski.modelMatrix, glm::radians(36.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(smiski.uniformLocations["model"], 1, GL_FALSE, glm::value_ptr(smiski.modelMatrix));

    glm::mat4 view = glm::mat4(1.0f);
    smiski.viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    glUniformMatrix4fv(smiski.uniformLocations["view"], 1, GL_FALSE, glm::value_ptr(smiski.viewMatrix));

    glm::mat4 modelView = view * smiski.modelMatrix;

    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));
    glUniformMatrix3fv(smiski.uniformLocations["normalMat"], 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glm::vec4 lightpos = view * lightPos;
    glUniform4fv(smiski.uniformLocations["uLightPos"], 1, glm::value_ptr(lightpos));

    // RENDER LOOP ----------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        // INPUT ---------------
        processInput(window);
        
        // MATRICES ---------
        smiski.use();
        smiski.projectionMatrix = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 1000.0f);
        glUniformMatrix4fv(smiski.uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(smiski.projectionMatrix));

        // ---------------- HDR BUFFER
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        smiski.use();
        glUniform1f(smiski.uniformLocations["uTime"], time);
        glUniform1f(smiski.uniformLocations["uStartTime"], startTime);
        glUniform1i(smiski.uniformLocations["uBlack"], black);
        glBindVertexArray(cube.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // BLUR --------------------------------
        bool horizontal = true, first = true;
        int amount = 20;
        
        blur.use();
        glActiveTexture(GL_TEXTURE0); 
        for (int i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
            // glActiveTexture(GL_TEXTURE0); //commented out -> creates a weird vertical extending blur effect, kinda cool
            glBindTexture(GL_TEXTURE_2D, first ? colorBuffer[1] : blurColor[!horizontal]);
            glUniform1i(blur.uniformLocations["uHorizontal"], horizontal);
            glBindVertexArray(quad.VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            horizontal = !horizontal;
            if (first) {
                first = false;
            }
        }

        // SCREEN FRAMEBUFFER
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdr.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer[0]);
        glUniform1i(glGetUniformLocation(hdr.ID, "hdrBuffer"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, blurColor[!horizontal]);
        glUniform1i(glGetUniformLocation(hdr.ID, "blurBuffer"), 1);
        glUniform1i(hdr.uniformLocations["uBlack"], black);

        glBindVertexArray(quad.VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // -----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // glDeleteBuffers(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glDeleteProgram(smiski.ID);
    glDeleteProgram(hdr.ID);
    glfwTerminate();
    return 0;
} //

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
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

GLFWwindow * initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "Glow in the Dark Shader", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failure to initialize window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    return window;
}
