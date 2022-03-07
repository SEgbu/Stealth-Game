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
        
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, 
        2, 3, 0
    };  

    // Creating the texture behaviour
    // Sets wrap mode for each axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Sets filtering mode for the textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // When minifying we use GL_LINEAR_MINIMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // When magnifying we use GL_LINEAR

    // Load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (!data){
        std::cout << "Failed to load container.jpg, stb error message: " << stbi_failure_reason() << std::endl;
    }

    // Create texture object
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Using minimap setting set previously
    stbi_image_free(data); // delete image data
    
    // Load second image
    data = stbi_load("assets/samurai.png", &width, &height, &nrChannels, 0);
    if (!data){
        std::cout << "Failed to load awesomeface.png, stb error message: " << stbi_failure_reason() << std::endl;
    }

    // Create second texture object
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Using minimap setting set previously
    
    stbi_image_free(data); // delete image data

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

    // Show where each attributes is in the vertex data and enable them.
    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0);
    // Texture attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Setting texture uniforms
    shaderProgram.use();
    shaderProgram.setInt("myTexture", 0);
    shaderProgram.setInt("myTexture2", 1);

    // Random cube positions 
    glm::vec3 cubePositions[] = {
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };


    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

        // Rendering commands
        glEnable(GL_DEPTH_TEST); // Enable depth
        glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // Set color which clears frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Set which buffer bit were clearing and for depth.

        shaderProgram.use(); // Start of shader program
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(VAO); // Bind VAO for this object

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        
        // Model matrix - responsible for transforming, scaling and/or rotations 
        for (unsigned int i = 0; i < 9; i++){
            // Randomly set transform for each cube
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            
            // Randomly set angle of each cube
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)(glm::radians(angle) * glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
            
            // Set Model matrix uniform
            int modelLoc = 0;
            modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // View matrix - acts as a camera
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // Projection matrix - changes the perspective 
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);
        // Set View matrix uniform
        int viewLoc = 0;
        viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Set projection matrix uniform
        int projectionLoc = 0;
        projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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