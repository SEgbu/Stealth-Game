#include <game.hpp>

// The game renderer
SpriteRenderer* renderer;

// The player 
GameObject* player;
GameObject* ground;

// Sets all public attributes to something
GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){

}

// deallocates renderer 
GameManager::~GameManager(){
    delete renderer;
    delete player;
    delete ground;
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
    renderer = new SpriteRenderer(ResourceManager::getShader("sprite").use());

    // loading and generating the player texture
    ResourceManager::loadTexture("assets/player.png", true, "player");

    // loading and generating the ground texture
    ResourceManager::loadTexture("assets/tg.png", true, "ground");

    // creating the player
    player = new GameObject(glm::vec2(75.0f, 100.0f), glm::vec2(25.0f, 50.0f), ResourceManager::getTexture("player"));

    // creating the ground
    ground = new GameObject(glm::vec2(30.0f, 300.0f), glm::vec2(300.f, 100.0f), ResourceManager::getTexture("ground"));
}

void GameManager::update(float deltaTime){

}

void GameManager::processInputs(float deltaTime){
    // when the game scene is on game active
    if (this->state == GAME_ACTIVE){
        // set velocity to the player velocity
        float velocity = PLAYER_VELOCITY * deltaTime;
        // if a or left is pressed
        if ((this->keys[GLFW_KEY_A] || this->keys[GLFW_KEY_LEFT])){
            // when the player is within screen bounds
            if (player->position.x >= 0.0f){
                // move the player to the left 
                player->position.x -= velocity;
            }
        }
        
        // if d or right is pressed
        if ((this->keys[GLFW_KEY_D] || this->keys[GLFW_KEY_RIGHT])){
            // when the player is within screen bounds
            if (player->position.x <= width - player->size.x){
                // move the player to the right
                player->position.x += velocity;
            }
        }
    }
}

void GameManager::render(){

    // render ground
    ground->draw(*renderer);
    // render player
    player->draw(*renderer);
}

