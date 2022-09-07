#pragma once

#include <iostream>

#include <box2d/box2d.h>
#include <box2d/b2_world.h>
#include <glm/glm/glm.hpp>

// Physics handler
class Physics{
    public:
        // constructor which sets the gravity
        Physics(float downwardsYForce);
        // destructor 
        ~Physics();
        // the creation of physics bodies in Box2D
        b2Body* createStaticBody(glm::vec2 position, glm::vec2 size);
        b2Body* createDynamicBody(glm::vec2 position, glm::vec2 size, float density, float friction);
    private:
        b2Vec2 gravity;
        static b2World* world; // world object for all box2d physics objects
};