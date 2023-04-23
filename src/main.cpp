// Predefined C++ Headers
#include <iostream>
#include <math.h>

// GLAD and GLFW Headers
#include <glad/glad.h>
#include <glfw/glfw3.h>

// SDL2 Libraries 
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Classes
#include <game.hpp>
#include <resources.hpp>

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

// GLFW functions
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Processes inputs
void processInput(GLFWwindow* window);

// The game manager
GameManager stealth(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* args[]){ 
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

    // Initializing SDL 
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Initializing SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << SDL_GetError() << std::endl;
    }

    // Set the size of the rendering viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set the change in the window size to GLFW
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Track key inputs using GLFW
    glfwSetKeyCallback(window, key_callback);

    // Track cursor position using GLFW
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    // initialize game manager
    stealth.init();

    // Render Loop
    while(!glfwWindowShouldClose(window)){

        // making the textures not look blurry
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // game loop functions
        if (stealth.quitProgram){
            break;
        }

        stealth.processInputs();
        stealth.update();
        
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        stealth.mousePos.xPos = xpos;
        stealth.mousePos.yPos = ypos;

        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
        if (state == GLFW_PRESS){
            stealth.leftClick = true;
        }
        else {
            stealth.leftClick = false;
        }

        // Rendering commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor((GLfloat)246/256, (GLfloat)184/256, (GLfloat)4/256, 1.0f); // Set color which clears frame

        stealth.render();
        // End of rendering commands

        glfwSwapBuffers(window); // Swaps front and back buffers
        glfwPollEvents(); // Checks if events are triggered
    }

    stealth.~GameManager();

    ResourceManager::clear(); // clear resources
    glfwTerminate(); // End program
    
    Mix_Quit();
    SDL_Quit();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // if esc and x is pressed then close window
    if ((glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_X)) && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // if key pressed is with the range of keys used in the game manager then it is valid
    if (key >= 0 && key < 1024){
        // if key within that range is pressed set key flag to true
        if (action == GLFW_PRESS){
            stealth.keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            stealth.keys[key] = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    
}