#include <game.hpp>

SpriteRenderer* renderer;

GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){

}

GameManager::~GameManager(){
    delete renderer;
}

void GameManager::init(){
    ResourceManager::loadShader("shaders/vertex.vs", "shaders/fragment.fs", "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    ResourceManager::getShader("sprite").use().setInteger("myTexture", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    ResourceManager::loadTexture("assets/player.png", true, "player");
}

void GameManager::update(float deltaTime){

}

void GameManager::processInputs(float deltaTime){

}

void GameManager::render(){
    renderer->drawSprite(ResourceManager::getTexture("player"), glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 400.0f));
}

