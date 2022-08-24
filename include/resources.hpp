#pragma once 

// C++ built-in libraries
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

// Local libraries
#include <shader.hpp>
#include <texture2D.hpp>

// static singleton class
class ResourceManager{
    public:
        // resource storage
        static std::map<std::string, Shader> shaders;
        static std::map<std::string, Texture2D> textures;

        // loading vertex, fragment shaders
        static Shader loadShader(const char* vShaderFilePath, const char* fShaderFilePath, std::string name);
        // get shader
        static Shader& getShader(std::string name);

        // loading texture
        static Texture2D loadTexture(const char* texFilePath, bool isTexAlpha, std::string name);
        // get texture
        static Texture2D& getTexture(std::string name);

        // deallocates resources properly
        static void clear();

    private:
        // private constructor for singleton class
        ResourceManager(){}

        // private methods for resource loading 
        static Shader loadShaderFromFile(const char* vShaderFilePath, const char* fShaderFilePath);
        static Texture2D loadTextureFromFile(const char* texFilePath, bool isTexAlpha);
};