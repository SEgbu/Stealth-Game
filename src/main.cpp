// Predefined C++ Headers
#include <iostream>
#include <math.h>

// GLAD and GLFW Headers
#include <glad/glad.h>
#include <glfw/glfw3.h>

// Shader class
#include <shader.hpp>

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
void framebufferSizeCallback(GLFWwindow* window, int width, int heigth);

// Processes inputs
void processInput(GLFWwindow* window);

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

    // Vertex data
    float vertices[] = {
        // positions          // colour 
        -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
        
    unsigned int indices[] = {  // note that we start from 0!
        0, 3, 1,
        0, 3, 2
    };  


    // Creating the element buffer object 
    unsigned int EBO; 
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Creating the vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Creating the vertex buffer object
    unsigned int VBO; // the id for the vertex buffer object
    glGenBuffers(1, &VBO); // Generating id for VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds to GL_ARRAY_BUFFER on OpenGL context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Initializing vertex buffer

    // Setup shader program
    Shader shaderProgram("shaders/vertex.vs", "shaders/fragment.fs");

    // Setup Vertex buffer pointer
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

        // Rendering commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // Set color which clears frame
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        shaderProgram.use(); // Start of shader program
        glBindVertexArray(VAO); // Bind VAO for this object

        // Draw triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO);

        glBindVertexArray(0);
        // End of rendering commands

        glfwSwapBuffers(window); // Swaps front and back buffers
        glfwPollEvents(); // Checks if events are triggered
    }

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
