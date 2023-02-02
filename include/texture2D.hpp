#pragma once 

#include <glad/glad.h>

class Texture2D{
    public:
        unsigned int wrapS, wrapT; // wrap modes on dimensions
        unsigned int internalFormat, imageFormat; // texture and image format
        unsigned int filterMin, filterMag; // filtering modes
        // constructor 
        Texture2D();
        Texture2D(unsigned int w, 
                  unsigned int h, 
                  unsigned int internalFormat, 
                  unsigned int imageFormat,
                  unsigned int wrapS = GL_REPEAT, 
                  unsigned int wrapT = GL_REPEAT,
                  unsigned int filterMin = GL_NEAREST,
                  unsigned int filterMag = GL_NEAREST);
        // make the texture
        void generate(unsigned int w, unsigned int h, unsigned char* data);
        // update the OpenGL context
        void bind() const; 
        // getters
        unsigned int ID;
        unsigned int width, height;      
};