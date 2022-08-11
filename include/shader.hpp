#pragma once

#include <glad/glad.h>
#include <string> 
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID; // the shader program ID
        Shader(){}
        Shader &use(); // sets shader active
        void compile(const char *vertexSource, const char *fragmentSource,
                     const char *geometrySource = nullptr); // compiles shaders
        void checkCompileErrors(unsigned int object, std::string type); // checks if compilation was good
        // utlity uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
};