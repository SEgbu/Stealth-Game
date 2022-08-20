#include <gamemanager.hpp>
#include <spriterenderer.hpp>
#include <resourcemanager.hpp>
#define STBI_NO_FAILURE_STRINGS

SpriteRenderer* renderer;

GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){
}

GameManager::~GameManager(){
    delete renderer;
}

void GameManager::init(){
    ResourceManager::loadShader("shaders/vertex.vs", "shaders/fragment.fs", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

    ResourceManager::getShader("sprite").use().setInt("myTexture", 0);
    glUniformMatrix4fv(glGetUniformLocation(ResourceManager::getShader("sprite").ID, "projection"), 1, false, glm::value_ptr(projection));

    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    ResourceManager::loadTexture("assets/player.png", true, "player");
}

void GameManager::processInputs(float deltaTime){

}

void GameManager::update(float deltaTime){

}

void GameManager::render(){
    renderer->drawSprite(ResourceManager::getTexture("player"), glm::vec2(0.0f, 0.0f), glm::vec2(200.0f, 400.0f));
}