#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

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

    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

    // Rendering commands
        glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // Set color which clears frame
        glClear(GL_COLOR_BUFFER_BIT); // Set which buffer bit were clearing (the colour one).

        // Vertex data
        float vertices[] = {
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.0f, -0.5f, 0.0f
        };

        // Creating the vertex buffer object
        unsigned int VBO; // the id for the vertex data
        glGenBuffers(1, &VBO); // Generating id for VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds to GL_ARRAY_BUFFER on OpenGL context
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Put vertex data onto vertex buffer

        // Vertex shader
        const char* vertexShaderCode = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main(){\n"
                                        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0)\n"
                                        "}\0";

        // Creating a vertex shader object
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); // Assign shader code on vertex shader
        glCompileShader(vertexShader);

        // Checking if vertex shader is successfully compiled
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success){
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "Error in vertex shader compilation, GLAD Shader Error Message: " << infoLog << std::endl;
        }

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