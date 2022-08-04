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
        // positions        // tex coords  
        -0.25f, 0.5f, 0.0f,  0.0f, 1.0f,  // top left
        0.25f, 0.5f, 0.0f,   1.0f, 1.0f,  // top right   
        -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
        0.25f, -0.5f, 0.0f,  1.0f, 0.0f   // bottom right   
    };
        
    unsigned int indices[] = {  // note that we start from 0!
        0, 3, 2,
        0, 3, 1
    };  

    // Creating the vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Creating the vertex buffer object
    unsigned int VBO; // the id for the vertex buffer object
    glGenBuffers(1, &VBO); // Generating id for VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds to GL_ARRAY_BUFFER on OpenGL context
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Initializing vertex buffer


    // Creating the texture object 
    unsigned int textureID; // Create the texture object ID 
    glGenTextures(1, &textureID); // Generate object using ID
    glBindTexture(GL_TEXTURE_2D, textureID); // Allow object to use a subset of the OpenGL context

    // Creating texture behaviour
    // Sets wrap mode for each texture axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set the colour of the border
    float borderColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
    // Set the filtering modes for the textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Load the image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char* data = stbi_load("assets/player.png", &width, &height, &nrChannels, 0);
    if (data == nullptr){
        std::cout << "Failed to load the image, STBI error function: " << stbi_failure_reason() << std::endl;
    }    
    // Put the image onto the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
    glGenerateMipmap(GL_TEXTURE_2D); // Generate minimap

    stbi_image_free(data); // Delete data since I don't need it anymore

    // Creating the element buffer object 
    unsigned int EBO; 
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Setup shader program
    Shader shaderProgram("shaders/vertex.vs", "shaders/fragment.fs");

    // Setup Vertex buffer pointer
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Tex Coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

        // Rendering commands
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set color which clears frame

        glBindVertexArray(VAO); // Bind VAO for this object
       
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
       
        shaderProgram.use(); // Start of shader program
        shaderProgram.setInt("myTexture", 0);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw triangles

        glBindVertexArray(0); 
        // End of rendering commanwds

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
