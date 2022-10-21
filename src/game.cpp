#include <game.hpp>
#include <noncollidable.hpp>
#include <vector>

// The game renderer
SpriteRenderer* renderer;

// The game objects
GameObject* player;
GameObject* ground;
GameObject* object;

// player crouching boolean
bool isPlayerCrouching = false;

// Sets all public attributes to something
GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){

}

// deallocates renderer 
GameManager::~GameManager(){
    delete renderer;
    delete player;
    delete ground;
}

// initialize variables 
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

    // loding and generating the player texture
    ResourceManager::loadTexture("assets/player.png", true, "player");

    // loading and generating the player crouch texture
    ResourceManager::loadTexture("assets/playerCrouch.png", true, "playerCrouch");

    // loading and generating the ground texture
    ResourceManager::loadTexture("assets/tg.png", true, "ground");

    // loading and genereating the object texture
    ResourceManager::loadTexture("assets/crate.png", true, "object");

    // list of player textures
    std::vector<Texture2D> playerTextures = {ResourceManager::getTexture("player"), ResourceManager::getTexture("playerCrouch")};
    
    // creating the player
    player = new GameObject(glm::vec2(75.0f, 100.0f), glm::vec2(25.0f, 50.0f), playerTextures);

    // creating player physics
    player->initPhysicsBody(true, 10.0f, 0.0f);

    // creating the ground
    ground = new GameObject(glm::vec2(375.0f, 600.0f), glm::vec2(700.f, 100.0f), ResourceManager::getTexture("ground"));

    // creating ground physics
    ground->initPhysicsBody(false);

    // creating the object
    object = new GameObject(glm::vec2(400.0f, 535.0f), glm::vec2(30.0f, 30.0f), ResourceManager::getTexture("object"));

    // create the object physics 
    object->initPhysicsBody(false);
}

void GameManager::update(){
    if (this->state == GAME_ACTIVE){
        // physics process per second
        float timeStep = 1.0f / 60.0f;

        /* the amount of iterations Box2D will do to 
        maintain the constraints on the physics body 
        in a single time step*/
        int32 velIterations = 8;
        int32 posIterations = 3;

        // execute the physics process for all physics objects
        player->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
        ground->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
        object->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
    }
}

void GameManager::processInputs(){
    // this check if the player makes contact with the ground
    bool groundCollision = false; 

    // when the game scene is on game active
    if (this->state == GAME_ACTIVE){

        // when the player collides with something it gets put into the contact list, so we iterate over this list
        for (b2ContactEdge* pcd = player->physicsBody->GetContactList(); pcd != NULL; pcd = pcd->next){
            // if the ground physics body is on the contact list then...
            if (pcd->other == ground->physicsBody && pcd->contact->IsTouching()){
                // ground collision occured
                groundCollision = true;
            }
            else {
                // ground collision didn't occur
                groundCollision = false;
            }
        }

        // helps me to only apply a force to the x axis
        float playerYVel = player->physicsBody->GetLinearVelocity().y;

        // if a or left is pressed
        if ((this->keys[GLFW_KEY_A] || this->keys[GLFW_KEY_LEFT])){
            if (isPlayerCrouching){
                // apply a slower impulse force in the left direction 
                player->physicsBody->ApplyLinearImpulse(b2Vec2(-(PLAYER_VELOCITY / 4), 0), player->physicsBody->GetWorldCenter(), true);
            }
            else {
                // apply a regular impulse force in the left direction
                player->physicsBody->ApplyLinearImpulse(b2Vec2(-PLAYER_VELOCITY, 0), player->physicsBody->GetWorldCenter(), true);
            }
        }
        // if d or right is pressed
        else if ((this->keys[GLFW_KEY_D] || this->keys[GLFW_KEY_RIGHT])){
            if (isPlayerCrouching){
                // apply a slower impulse force in right direction
                player->physicsBody->ApplyLinearImpulse(b2Vec2(PLAYER_VELOCITY / 4, 0), player->physicsBody->GetWorldCenter(), true);
            }
            else {
                // apply a regular impulse force in the right direction
                player->physicsBody->ApplyLinearImpulse(b2Vec2(PLAYER_VELOCITY, 0), player->physicsBody->GetWorldCenter(), true);
            }
        }
        else{
            // stop the player horizontally
            player->physicsBody->SetLinearVelocity(b2Vec2(0, playerYVel));
        }

        // if w is pressed and player's y velocity = 0 and player presses once
        if (this->keys[GLFW_KEY_W] && (abs(playerYVel) <= 0.1) && groundCollision){
            player->physicsBody->ApplyLinearImpulse(b2Vec2(0, -10000.0f), player->physicsBody->GetWorldCenter(), true);
        }

        // sets the crouching boolean true if the s key is pressed
        if (this->keys[GLFW_KEY_S]){
            isPlayerCrouching = true;
        }
        else {
            isPlayerCrouching = false;
        }
    }
}

void GameManager::render(){
    
    // render ground
    ground->draw(*renderer, -1, 0);

    // render player
    if (isPlayerCrouching){
        player->draw(*renderer, 1, 0);
    }
    else{
        player->draw(*renderer, 0, 0);
    }
    // render object 
    object->draw(*renderer, -1, 0);
}
