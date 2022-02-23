#include <iostream>
#include <math.h>
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

    // Vertex data
    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top     
    };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2
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

    // Vertex shader
    const char* vertexShaderCode = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "layout (location = 1) in vec3 aColour;\n"
                                    "out vec3 outColour;\n"
                                    "void main(){\n"
                                    "gl_Position = vec4(aPos, 1.0);\n"
                                    "outColour = aColour;\n"
                                    "}\0";

    // Creating a vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); // Assign shader code on vertex shader
    glCompileShader(vertexShader);

    // Checking if vertex shader is successfully compiled
    int vertexShaderComplitationSuccess;
    char vertexShaderInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderComplitationSuccess);

    if (!vertexShaderComplitationSuccess){
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
        std::cout << "Error in vertex shader compilation, GLAD Error Message: " << vertexShaderInfoLog << std::endl;
    }

    // Fragment Shader
    const char* fragmentShaderCode = "#version 330 core\n"
                                        "out vec4 fragColour;\n"
                                        "in vec3 outColour;\n"
                                        "void main(){\n"
                                        "fragColour = vec4(outColour, 1.0);\n"
                                        "}\0";

    // Initialize the fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    // Check if fragment shader compiled properly
    int fragmentShaderCompilationSuccess;
    char fragmentShaderInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompilationSuccess);

    if (!fragmentShaderCompilationSuccess){
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
        std::cout << "Error when compiling fragment shader, GLAD error message: " << fragmentShaderInfoLog << std::endl;
    }

    // Initializing the shader program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if shader program object linked shaders properly
    int shaderProgramLinkingSuccess;
    char shaderProgramInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramLinkingSuccess);
    
    if (!shaderProgramLinkingSuccess){
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
        std::cout << "Error message when linking shaders, GLAD error message" << shaderProgramInfoLog << std::endl;
    }

    // Delete shader since we don't need them
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    // Determines vertex attributes should be interpretted by creating the vertex buffer data forma and enable attributes.
    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0);
    // Colour attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render Loop
    while(!glfwWindowShouldClose(window)){
        processInput(window); // Inputs

        // Rendering commands
        glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // Set color which clears frame
        glClear(GL_COLOR_BUFFER_BIT); // Set which buffer bit were clearing (the colour one)

        glUseProgram(shaderProgram); // Start of shader program
        glBindVertexArray(VAO); // Bind VAO for this object

        // Changing the green value of our rectangle
        float time = glfwGetTime();
        float greenValue = (sin(time * 2) / 2.0f) + 0.5f;
        int vertexColourLocation = glGetUniformLocation(shaderProgram, "colour");
        glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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