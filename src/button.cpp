#include <button.hpp>

void Button::init(){
    isHover = false;
    isPressed = false;
}

void Button::eventHandler(double xPos, double yPos, bool isLeftClicked){
    // button hover event
    if (
        xPos > this->position.x &&
        xPos < (this->position.x + this->size.x) &&
        yPos > this->position.y &&
        yPos < (this->position.y + this->size.y) && 
        !isLeftClicked
    ){
        isHover = true;
        isPressed = false;
    }
    // button press event
    else if(
        xPos > this->position.x &&
        xPos < (this->position.x + this->size.x) &&
        yPos > this->position.y &&
        yPos < (this->position.y + this->size.y) && 
        isLeftClicked 
    ){
        isHover = false;
        isPressed = true;
    }
    // button idle event
    else {
        isHover = false;
        isPressed = false;
    }
}