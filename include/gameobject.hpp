#pragma once 

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

// Local Libraries
#include <texture2D.hpp>
#include <sprite.hpp>

class GameObject {
    public:
        // object attributes
        glm::vec2 position, size, velocity;
        glm::vec3 colour;
        float rotation;

        // sprite of game object
        Texture2D sprite;

        // constructors
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col = glm::vec3(1.0f), 
                   glm::vec2 vel = glm::vec2(0.0f, 0.0f));
        // draw sprite 
        virtual void draw(SpriteRenderer &renderer);
};