#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include <texture2D.hpp>
#include <shader.hpp>

/* 
This is a static (when one change happens to an object members it happens
to all the object's members) singleton (a class which can only have
one object) class
*/
class ResourceManager{
    public:
        // resource storage
        static std::map<std::string, Shader> shaders;
        static std::map<std::string, Texture2D> textures;
        // loading vertex, fragment (and geometry) shaders
        static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        // get stored shader
        static Shader& getShader(std::string name);
        // loading texture
        static Texture2D loadTexture(const char* file, bool alpha, std::string name);
        // get stored texture
        static Texture2D& getTexture(std::string name);
        // deallocates resources
        static void clear();
    private: 
        // private constructor to prevent resourceManager objects
        ResourceManager(){}
        // private methods for resource loading
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static Texture2D loadTextureFromFile(const char* file, bool alpha);
};