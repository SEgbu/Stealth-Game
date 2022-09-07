#include <gameobject.hpp>


// physics handler
Physics physics(0.45f);

GameObject::GameObject(): position(0.0f, 0.0f), size(1.0f, 1.0f), velocity(0.0f),
                          colour(1.0f), rotation(0.0f), sprite(){
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col, glm::vec2 vel): 
                       position(pos), size(s), sprite(spr), colour(col), velocity(vel) {
} 

GameObject::~GameObject(){
    physicsBody = nullptr; // deallocate physics body
}

void GameObject::initPhysicBody(bool isDynamic, float density, float friction){
    // if a dynamic physics body is requested
    if (isDynamic){
        // create a dynamic physics body
        physicsBody = physics.createDynamicBody(position, size, density, friction);
    }
    // if not...
    else {
        // create a static physics body
        physicsBody = physics.createStaticBody(position, size);
    }
}

void GameObject::draw(SpriteRenderer &renderer){
    float offsetX = size.x / 2;
    float offsetY = size.y / 2;
    // render the sprite
    renderer.drawSprite(this->sprite, glm::vec2(physicsBody->GetPosition().x - offsetX, physicsBody->GetPosition().y - offsetY), this->size, this->rotation, this->colour);
}