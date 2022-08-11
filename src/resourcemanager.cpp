#include <resourcemanager.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

// instatiate static variables
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name){
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name]; 
}

Shader ResourceManager::getShader(std::string name){
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name){
    textures[name] = loadTextureFromFile(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name){
    return textures[name];
}

void ResourceManager::clear(){
    // delete all textures
    for (auto i : textures){
        glDeleteProgram(i.second.ID);
    }
    // delte all shaders
    for (auto i : shaders){
        glDeleteProgram(i.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr){
    // get vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try{
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close files
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert buffer contents to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // do the same this for the geometry shader if != nullptr
        if (gShaderFile != nullptr){
            std::ifstream geometryShaderFile;
            std::stringstream geometryShaderStream;
            geometryShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = geometryShaderStream.str();
        }
    }
    catch(std::exception e){
        std::cout << "loadShaderFromFile error: failed to read shader files" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}