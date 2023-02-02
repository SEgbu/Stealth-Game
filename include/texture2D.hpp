#pragma once 

#include <glad/glad.h>

class Texture2D{
    public:
        unsigned int wrapS, wrapT; // wrap modes on dimensions
        unsigned int internalFormat, imageFormat; // texture and image format
        unsigned int filterMin, filterMag; // filtering modes
        // constructor 
        Texture2D();
        // make the texture
        void generate(unsigned int w, unsigned int h, unsigned char* data);
        // update the OpenGL context
        void bind() const; 
        // getters
        unsigned int ID;
        unsigned int width, height;      
};