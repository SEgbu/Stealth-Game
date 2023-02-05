#pragma once

#include <noncollidable.hpp>
#include <glfw/glfw3.h>
#include <glm/glm/glm.hpp>

class Button : public NonCollidableObject{
    public: 
        using NonCollidableObject::NonCollidableObject;
        void init();
        void eventHandler(double xPos, double yPos, bool isLeftClicked);
        bool isHover;
        bool isPressed;
};

