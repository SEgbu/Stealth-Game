#pragma once

#include <gameobject.hpp>

class NonCollidableObject : public GameObject{
    public:
        // object attributes
        glm::vec2 position, size, velocity;
        glm::vec3 colour;
        float rotation;

        // sprite of game object
        Texture2D sprite;
        std::vector<Texture2D> spriteVector;

        // constructor
        NonCollidableObject();
        NonCollidableObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col = glm::vec3(1.0f), 
                   glm::vec2 vel = glm::vec2(0.0f, 0.0f));
        NonCollidableObject(glm::vec2 pos, glm::vec2 s, std::vector<Texture2D> spr2, glm::vec3 col = glm::vec3(1.0f), 
                   glm::vec2 vel = glm::vec2(0.0f, 0.0f));

        // polymorphic render function
        void draw(SpriteRenderer& renderer, int texIndex, int zIndex);
};