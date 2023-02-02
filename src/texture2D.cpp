#include <texture2D.hpp>
#include <iostream>

Texture2D::Texture2D() : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB),
                         wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_NEAREST), filterMag(GL_NEAREST){
}

Texture2D::Texture2D(unsigned int w, unsigned int h, unsigned int internalFormat, unsigned int imageFormat, unsigned int wrapS, unsigned int wrapT,
                     unsigned int filterMin, unsigned int filterMag) : width(w), height(h), internalFormat(internalFormat), imageFormat(imageFormat),
                                                                       wrapS(wrapS), wrapT(wrapT), filterMin(filterMin), filterMag(filterMag){

}

void Texture2D::generate(unsigned int w, unsigned int h, unsigned char* data){
    this->width = w;
    this->height = h;

    // set up texture ID
    glGenTextures(1, &this->ID);

    // open texture settings
    glBindTexture(GL_TEXTURE_2D, this->ID);

    // generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

    // create texture behaviour
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

    // close texture settings
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const{
    glBindTexture(GL_TEXTURE_2D, this->ID);
}