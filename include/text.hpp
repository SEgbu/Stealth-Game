#pragma once

#include <iostream>
#include <map>
#include <glm/glm/glm.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include <shader.hpp>
#include FT_FREETYPE_H
#include <glm/glm/gtc/matrix_transform.hpp>
#include <texture2D.hpp>


class Text {
    public:
        Text(char* ffp); // set ffp to the font's file path
        ~Text(); // deinitialize freetype
        bool Init(float scrwidth, float scrheight); // initializes freetype and loads fonts
        void Render(std::string text, float x, float y, float scale, glm::vec3 colour); // renders text
    private:
        char* fontFilePath;
        FT_Library ftLibrary;
        FT_Face face;
        unsigned int VAO, VBO;
        Shader shader;
};