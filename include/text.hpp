#pragma once 

#include <vector>

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

// Local Libraries
#include <resources.hpp>
#include <sprite.hpp>

class Text {
    public:
        glm::vec2 position;
        glm::vec2 fontSize;

        Text(glm::vec2 pos, glm::vec2 fSize);
        void draw(SpriteRenderer& renderer, std::string message);
    private:
        std::vector<Texture2D> alphabetTextures;
};