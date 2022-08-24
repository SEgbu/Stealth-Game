#include <game.hpp>

// The game renderer
SpriteRenderer* renderer;

// Sets all public attributes to something
GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){

}

// deallocates renderer 
GameManager::~GameManager(){
    delete renderer;
}

void GameManager::init(){
    // setting up shader program
    ResourceManager::loadShader("shaders/vertex.vs", "shaders/fragment.fs", "sprite"); 

    // setting up projection matrix for the player viewing the game 
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    // setting some uniforms in the vertex or frag shader
    ResourceManager::getShader("sprite").use().setInteger("myTexture", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    
    // setting up the game renderer
    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    // loading and generating the player texture
    ResourceManager::loadTexture("assets/player.png", true, "player");
}

void GameManager::update(float deltaTime){

}

void GameManager::processInputs(float deltaTime){

}

void GameManager::render(){
    // render player
    renderer->drawSprite(ResourceManager::getTexture("player"), glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 400.0f));
}

