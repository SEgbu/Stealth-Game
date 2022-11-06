#pragma once 
#include <noncollidable.hpp>

class Trigger : public NonCollidableObject {
    public:
        using NonCollidableObject::NonCollidableObject; // using the base classes constructor 
        bool isTriggerIntersecting(Trigger otherTrigger); // checks whether the trigger is intersecting with another trigger
};

