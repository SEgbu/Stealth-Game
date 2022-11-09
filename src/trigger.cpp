#include <trigger.hpp>

bool Trigger::isTriggerIntersecting(Trigger* otherTrigger){
    bool intersection = false;
    
    if ((this->position.x < (otherTrigger->position.x + otherTrigger->size.x)) &&
       ((this->position.x + this->size.x) > otherTrigger->position.x) &&
       (this->position.y < (otherTrigger->position.y + otherTrigger->size.y)) && 
       ((this->position.y + this->size.y) > otherTrigger->position.y)){
            intersection = true;
    }

    return intersection;
}