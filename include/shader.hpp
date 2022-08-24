#pragma once

// IO stream
#include <iostream>

// OpenGL Math libraries
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>

// GLAD library
#include <glad/glad.h>


class Shader{
    public: 
        unsigned int ID;

        Shader(){};
        Shader &use(); // set shaders active
        void compile(const char *vertexSource, const char* fragmentSource); // compile v & f shader
        void checkCompileErrors(unsigned int obj, std::string type); // error checking for shader compilation
        
        // utility uniform setting functions
        void setFloat    (const char *name, float value, bool useShader = false);
        void setInteger  (const char *name, int value, bool useShader = false);
        void setVector2f (const char *name, float x, float y, bool useShader = false);
        void setVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
        void setVector3f (const char *name, float x, float y, float z, bool useShader = false);
        void setVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
        void setVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
        void setMatrix4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
};