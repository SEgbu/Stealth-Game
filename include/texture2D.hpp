#pragma once 
#include <glad/glad.h>

class Texture2D {
    public:
        unsigned int wrapS, wrapT; // wrapping mode dimensions
        unsigned int internalFormat, imageFormat; // texture and image format
        unsigned int filterMin, filterMag; // filtering modes
        // constructor
        Texture2D();
        // generate the texture 
        void generate(unsigned int w, unsigned int h, unsigned char* data);
        // bind the texture to the OpenGL context
        void bind() const;
        // getters 
        unsigned int ID; // used to access the OpenGL context
        unsigned int width, height; // dimensions of the texture
};