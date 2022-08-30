#include <gameobject.hpp>


GameObject::GameObject(): position(0.0f, 0.0f), size(1.0f, 1.0f), velocity(0.0f), 
                          colour(1.0f), rotation(0.0f), sprite(){}

GameObject::GameObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col, glm::vec2 vel): 
                       position(pos), size(s), sprite(spr), colour(col), velocity(vel) {}

void GameObject::draw(SpriteRenderer &renderer){
    renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->colour);
}