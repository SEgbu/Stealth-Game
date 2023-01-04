#include <noncollidable.hpp>

NonCollidableObject::NonCollidableObject(): position(0.0f, 0.0f), size(1.0f, 1.0f), velocity(0.0f),
                          colour(1.0f), rotation(0.0f), sprite(){
    rotation = 0.0f;

}

NonCollidableObject::NonCollidableObject(glm::vec2 pos, glm::vec2 s, Texture2D spr, glm::vec3 col, glm::vec2 vel): 
                       position(pos), size(s), sprite(spr), colour(col), velocity(vel) {
    rotation = 0.0f;
} 

NonCollidableObject::NonCollidableObject(glm::vec2 pos, glm::vec2 s, std::vector<Texture2D> spr2, glm::vec3 col, glm::vec2 vel): 
                       position(pos), size(s), spriteVector(spr2), colour(col), velocity(vel) {
    rotation = 0.0f;

} 


void NonCollidableObject::draw(SpriteRenderer& renderer, int texIndex, int zIndex){
    // render the sprite    
    if (texIndex == -1)
        renderer.drawSprite(sprite, position, zIndex, size, rotation, colour);       
    else
        renderer.drawSprite(spriteVector[texIndex], position, zIndex, size, rotation, colour);
}