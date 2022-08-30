#pragma once

#include <box2d-main/include/box2d/box2d.h>
#include <glm/glm/glm.hpp>

class Physics{
    public:
        Physics(float downwardsYForce);
        void init();
        b2Body* createStaticBody(glm::vec2 position, glm::vec2 size);
        b2Body* createDynamicBody(glm::vec2 position, glm::vec2 size, float density, float friction);
    private:
        b2Vec2 gravity;
};