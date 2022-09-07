#include <resources.hpp>

#include "../include/stb/stb_image.h"

// instantiate static variables
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

Shader ResourceManager::loadShader(const char* vShaderFilePath, const char* fShaderFilePath, std::string name){
    shaders[name] = loadShaderFromFile(vShaderFilePath, fShaderFilePath); // create shader program for specific object
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* texFilePath, bool isTexAlpha, std::string name){
    textures[name] = loadTextureFromFile(texFilePath, isTexAlpha); // create texture for specific object
    return textures[name];
}

Shader ResourceManager::getShader(std::string name){
    return shaders[name];
}

Texture2D ResourceManager::getTexture(std::string name){
    return textures[name];
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFilePath, const char* fShaderFilePath){
    // shader source code in string format
    std::string vertexShaderStr;
    std::string fragShaderStr;

    // exception handling 
    try {
        // taking in shader files for reading 
        std::ifstream vertexShaderFile(vShaderFilePath);
        std::ifstream fragShaderFile(fShaderFilePath);
        // initializing shader streams (a flow of data coming in and out of a piece of memory).
        std::stringstream vertexShaderStream, fragShaderStream; 

        // putting shader file buffers onto shader string streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragShaderStream << fragShaderFile.rdbuf();

        // i don't need these files anymore
        vertexShaderFile.close();
        fragShaderFile.close();

        // getting source code strings from the string stream.
        vertexShaderStr = vertexShaderStream.str();
        fragShaderStr = fragShaderStream.str();
    }
    // if an exception is detected then print out an error on the error stream
    catch (std::exception e){
        std::cerr << "Failed to load shader from file" << std::endl;
    }

    // convert the string to const character pointers (the format which OpenGL likes)
    const char* vertexShaderCode = vertexShaderStr.c_str();
    const char* fragShaderCode = fragShaderStr.c_str();

    // compile shaders
    Shader shader;
    shader.compile(vertexShaderCode, fragShaderCode);

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* texFilePath, bool isTexAlpha){
    Texture2D texture; // final texture

    // checks if it is a png (has transparency)
    if (isTexAlpha){
        // if so, change the formats to RGBA
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    // loading the image data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texFilePath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data == nullptr){
        std::cout << "Failed to load image, stbi error function: " << stbi_failure_reason() << std::endl;
    }

    // generating the texture
    texture.generate(width, height, data);

    // i don't need the image data anymore so deallocate it in memory
    stbi_image_free(data);

    return texture;
}

// properly deallocating shader and texture maps
void ResourceManager::clear(){
    for (auto iter : shaders){
        glDeleteProgram(iter.second.ID);
    }

    for (auto iter : textures){
        glDeleteTextures(1, &iter.second.ID);
    }
}