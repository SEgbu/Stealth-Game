#pragma once

#include <vector>

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

// Local Libraries
#include <texture2D.hpp>
#include <sprite.hpp>
#include <physics.hpp>

class GameObject {
    public:
        // object attributes
        glm::vec2 position, size, velocity;
        glm::vec3 colour;
        float rotation;

        // sprite of game object
        Texture2D sprite;
        std::vector<Texture2D> spriteVector;

        // Box2D physics body
        b2Body* physicsBody;  


        // constructors
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col = glm::vec3(1.0f), 
                   glm::vec2 vel = glm::vec2(0.0f, 0.0f));
        GameObject(glm::vec2 pos, glm::vec2 s, std::vector<Texture2D> spr2, glm::vec3 col = glm::vec3(1.0f), 
                   glm::vec2 vel = glm::vec2(0.0f, 0.0f));
        // destructors
        ~GameObject();
        // initialize physics body
        void initPhysicsBody(bool isDynamic, float density = 0.0f, float friction = 0.0f);
        // draw specific sprite
        virtual void draw(SpriteRenderer &renderer, int texIndex, int zIndex);
};