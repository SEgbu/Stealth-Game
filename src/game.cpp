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

    // creating player physics
    player->initPhysicBody(true, 10.0f, 0.0f);

    // creating the ground
    ground = new GameObject(glm::vec2(375.0f, 600.0f), glm::vec2(700.f, 100.0f), ResourceManager::getTexture("ground"));

    // creating ground physics
    ground->initPhysicBody(false);

    // ground box collider position debug
    std::cout << ground->physicsBody->GetPosition().x << "," << ground->physicsBody->GetPosition().y << std::endl;
}

void GameManager::update(float deltaTime){
    // physics process per second
    float timeStep = 1.0f / 60.0f;

    /* the amount of iterations Box2D will do to 
    maintain the constraints on the physics body 
    in a single time step*/
    int32 velIterations = 6;
    int32 posIterations = 2;

    // execute the physics process for all physics objects
    player->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
    ground->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
}

void GameManager::processInputs(float deltaTime){
    // when the game scene is on game active
    if (this->state == GAME_ACTIVE){
        // helps me to only apply a force to the x axis
        float playerYVel = player->physicsBody->GetLinearVelocity().y;

        // if a or left is pressed
        if ((this->keys[GLFW_KEY_A] || this->keys[GLFW_KEY_LEFT])){
            // apply impulse force in the left direction
            player->physicsBody->ApplyLinearImpulse(b2Vec2(-PLAYER_VELOCITY, 0), player->physicsBody->GetWorldCenter(), true);
        }
        // if d or right is pressed
        else if ((this->keys[GLFW_KEY_D] || this->keys[GLFW_KEY_RIGHT])){
            // apply impulse force in right direction
            player->physicsBody->ApplyLinearImpulse(b2Vec2(PLAYER_VELOCITY, 0), player->physicsBody->GetWorldCenter(), true);
        }
        else{
            // stop the player horizontally
            player->physicsBody->SetLinearVelocity(b2Vec2(0, playerYVel));
        }

        // if w is pressed and player's y velocity = 0
        if (this->keys[GLFW_KEY_W] && (playerYVel <= 0.00001) ){
            player->physicsBody->ApplyLinearImpulse(b2Vec2(0, 30.0f), player->physicsBody->GetLinearVelocity(), true);
        }
    }
}

void GameManager::render(){
    // render ground
    ground->draw(*renderer);
    // render player
    player->draw(*renderer);
}

