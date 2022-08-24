#include <resources.hpp>

#include "../include/stb/stb_image.h"

// instantiate static variables
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

Shader ResourceManager::loadShader(const char* vShaderFilePath, const char* fShaderFilePath, std::string name){
    shaders[name] = loadShaderFromFile(vShaderFilePath, fShaderFilePath);
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* texFilePath, bool isTexAlpha, std::string name){
    textures[name] = loadTextureFromFile(texFilePath, isTexAlpha);
    return textures[name];
}

Shader& ResourceManager::getShader(std::string name){
    return shaders[name];
}

Texture2D& ResourceManager::getTexture(std::string name){
    return textures[name];
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFilePath, const char* fShaderFilePath){
    std::string vertexShaderStr;
    std::string fragShaderStr;

    try {
        std::ifstream vertexShaderFile(vShaderFilePath);
        std::ifstream fragShaderFile(fShaderFilePath);
        std::stringstream vertexShaderBuffer, fragShaderBuffer;

        vertexShaderBuffer << vertexShaderFile.rdbuf();
        fragShaderBuffer << fragShaderFile.rdbuf();

        vertexShaderFile.close();
        fragShaderFile.close();

        vertexShaderStr = vertexShaderBuffer.str();
        fragShaderStr = fragShaderBuffer.str();
    }
    catch (std::exception e){
        std::cout << "Failed to load shader from file" << std::endl;
    }

    const char* vertexShaderCode = vertexShaderStr.c_str();
    const char* fragShaderCode = fragShaderStr.c_str();

    Shader shader;
    shader.compile(vertexShaderCode, fragShaderCode);

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* texFilePath, bool isTexAlpha){
    Texture2D texture;

    if (isTexAlpha){
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texFilePath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data == nullptr){
        std::cout << "Failed to load image, stbi error function: " << stbi_failure_reason() << std::endl;
    }

    texture.generate(width, height, data);

    stbi_image_free(data);

    return texture;
}

void ResourceManager::clear(){
    for (auto iter : shaders){
        glDeleteProgram(iter.second.ID);
    }

    for (auto iter : textures){
        glDeleteTextures(1, &iter.second.ID);
    }
}