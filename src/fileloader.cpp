#include <fileloader.hpp>

#include <iostream>
#include <stb/stb_image.h>

unsigned char* imageLoader(const char* file, int* width, int* height, int* nrChannels){
    unsigned char* data = stbi_load(file, width, height, nrChannels, 0);
    std::cout << (const char*)data << " " << file << " " << width << " " << height << " " << nrChannels;
    if (data == nullptr)
        std::cout << "Failed to load image, STBI failure reason: " << stbi_failure_reason() << std::endl;
        return nullptr;

    return data;
}