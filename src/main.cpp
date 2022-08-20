// Predefined C++ Headers
#include <iostream>
#include <math.h>

// GLAD and GLFW Headers
#include <glad/glad.h>
#include <glfw/glfw3.h>

// Classes
#include <gamemanager.hpp>
#include <resourcemanager.hpp>

// STB Image Loader Headers
#define STB_IMAGE_IMPLEMENTATION 
#include <stb/stb_image.h>

// OpenGL Mathematics Headers
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// This changes the size of the viewport as the window is resized.
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// Processes inputs
void processInput(GLFWwindow* window);

GameManager game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(){ 
    // Initializing GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating the window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stealth Game", NULL, NULL);
    if (window == NULL){
        std::cout << "GLFW failed to create window" << std::endl;
        glfwTerminate();
    }  
    glfwMakeContextCurrent(window); // Creates the OpenGL context for window

    // Initializing GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ // Gets address of the function from this context
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the size of the rendering viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set the change in the window size to GLFW
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    game.init();

    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

        // Deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        game.processInputs(deltaTime);
        game.update(deltaTime);

        // Rendering commands

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set color which clears frame

        game.render();
        // End of rendering commands

        glfwSwapBuffers(window); // Swaps front and back buffers
        glfwPollEvents(); // Checks if events are triggered
    }

    ResourceManager::clear();
    glfwTerminate(); // End program
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_X)){
        glfwSetWindowShouldClose(window, true);
    }
}
