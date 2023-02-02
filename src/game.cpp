#include <game.hpp>
#include <noncollidable.hpp>
#include <trigger.hpp>
#include <vector>
#include <text.hpp>

// The game renderer
SpriteRenderer* renderer;

// The game objects
GameObject* player;
GameObject* ground;
GameObject* object;
GameObject* enemy;

Trigger* enemyDetectionZone;
Trigger* enemyBack;
Trigger* playerTrigger;
Trigger* enemyJumpTrigger;
Trigger* boxTrigger;

Text testText((char*)"assets/SofiaSans-Bold.tff");

NonCollidableObject* exclamationMark;

// enumerator for enemy state
enum EnemyState {
    IDLE, 
    AGGRO
};

// enemy default state is idle
EnemyState enemyState = IDLE;

// player crouching boolean
bool isPlayerCrouching = false;

// player trigger size change boolean
bool isPlayerTriggerSizeChanged = false;

// a boolean which checks whether the player has intersected with the enemy
bool hasPlayerKilledEnemy = false;

// setting current time to zero 
double time = 0;

// a boolean to check whether the enemy has changed direction
bool hasEnemyChangedDir = false;

// a boolean to check whether the enemy has changed direction in the aggressive state 
bool hasEnemyChangeDirInAggro = false;

// a boolean which checks if the enemy has jumped yet
bool hasEnemyJumped = false;

// Sets all public attributes to something
GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_ACTIVE){
    std::cout << "djalfsjflsj";
}

// deallocates renderer 
GameManager::~GameManager(){
    delete renderer;
    delete player;
    delete ground;
    delete object;
    delete enemy;

    delete enemyDetectionZone;
    delete enemyBack; 
    delete playerTrigger;
    delete enemyJumpTrigger;
    delete boxTrigger;

    delete exclamationMark;
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

    // loading and generating the enemy left texture
    ResourceManager::loadTexture("assets/enemyLeft.png", true, "enemyLeft");

    // loading and generating the enemy right texture 
    ResourceManager::loadTexture("assets/enemyRight.png", true, "enemyRight");

    // loading and generating the enemy trigger texture 
    ResourceManager::loadTexture("assets/trigger.png", true, "enemyDetectionZone");

    // loading and generating the player's trigger texture
    ResourceManager::loadTexture("assets/playerTrigger.png", true, "playerTrigger");

    // loading and generating the enemy's back trigger texture
    ResourceManager::loadTexture("assets/enemyBackTrigger.png", true, "enemyBack");

    // loading and generating the enemy's jump trigger texture
    ResourceManager::loadTexture("assets/enemyJumpTrigger.png", true, "enemyJump");

    // loading and generating the box's trigger texture
    ResourceManager::loadTexture("assets/boxTrigger.png", true, "box");

    // loading and generating the exclamation mark 
    ResourceManager::loadTexture("assets/exclamation.png", true, "exclamation");

    // list of player textures
    std::vector<Texture2D> playerTextures = {ResourceManager::getTexture("player"), ResourceManager::getTexture("playerCrouch")};

    // list of enemy textures
    std::vector<Texture2D> enemyTextures = {ResourceManager::getTexture("enemyLeft"), ResourceManager::getTexture("enemyRight")};
    
    // creating the player
    player = new GameObject(glm::vec2(75.0f, 100.0f), glm::vec2(25.0f, 50.0f), playerTextures);

    // creating player physics
    player->initPhysicsBody(true, 10.0f, 0.0f);

    // create the enemy
    enemy = new GameObject(glm::vec2(500.0f, 75.0f), glm::vec2(25.0f, 50.0f), enemyTextures);

    // create the enemy physics
    enemy->initPhysicsBody(true, 10.0f, 0.0f);

    // creating enemy trigger
    enemyDetectionZone = new Trigger(enemy->position, glm::vec2(220.0f, 5.0f), ResourceManager::getTexture("enemyDetectionZone"));

    // creating player trigger 
    playerTrigger = new Trigger(player->position, player->size, ResourceManager::getTexture("playerTrigger"));

    // creating enemy back trigger 
    enemyBack = new Trigger((enemy->position + enemy->size), glm::vec2(5.0f, enemy->size.y), ResourceManager::getTexture("enemyBack"));

    // creating enemy jump trigger 
    enemyJumpTrigger = new Trigger(enemy->position, enemyBack->size, ResourceManager::getTexture("enemyJump"));

    // creating the exclamation mark
    exclamationMark = new NonCollidableObject(glm::vec2(0,0), glm::vec2(14.0f, 28.0f), ResourceManager::getTexture("exclamation"));

    // creating the ground
    ground = new GameObject(glm::vec2(375.0f, 600.0f), glm::vec2(700.f, 100.0f), ResourceManager::getTexture("ground"));

    // creating ground physics
    ground->initPhysicsBody(false);

    // creating the object
    object = new GameObject(glm::vec2(400.0f, 535.0f), glm::vec2(30.0f, 30.0f), ResourceManager::getTexture("object"));

    // create the object physics 
    object->initPhysicsBody(false);

    // creating the box trigger
    boxTrigger = new Trigger(glm::vec2(object->physicsBody->GetPosition().x, object->physicsBody->GetPosition().y) - (object->size.y / 2), object->size, ResourceManager::getTexture("box"));

    // // initialise text
    testText.Init(this->width, this->height);
    
    // set time to zero 
    glfwSetTime(time);
}

void GameManager::update(){
    if (this->state == GAME_ACTIVE){
        // physics process per second
        float timeStep = 1.0f / 60.0f;

        /* the amount of iterations Box2D will do to 
        maintain the constraints on the physics body 
        in a single time step*/
        int32 velIterations = 8;
        int32 posIterations = 8;

        // make the player trigger follow the player on the x
        playerTrigger->position.x = player->physicsBody->GetPosition().x - (player->size.x / 2);
        
        // if player is crouching and player's trigger size hasn't changed
        if (isPlayerCrouching && !isPlayerTriggerSizeChanged){
            // change trigger size to half the player's height
            playerTrigger->size.y /= 2;
            // make the player trigger follow the player
            playerTrigger->position.y = player->physicsBody->GetPosition().y;
            // trigger size has changed
            isPlayerTriggerSizeChanged = true;
        }
        // if player isn't crouching
        else if (!isPlayerCrouching){
            // set player back to its original height 
            playerTrigger->size.y = player->size.y;
            // make the player trigger follow the player on the y
            playerTrigger->position.y = player->physicsBody->GetPosition().y - (player->size.y / 2);
            // trigger size become or remains unchanged
            isPlayerTriggerSizeChanged = false;
        }

        // turns off enemy triggers is enemy is killed by player
        if (playerTrigger->isTriggerIntersecting(enemyBack) && !hasPlayerKilledEnemy){
            enemy->physicsBody->GetFixtureList()->SetSensor(true);
            hasPlayerKilledEnemy = true;
        }

        // enemy idle state
        if (enemyState == IDLE){
            // if the enemy is idle, let the enemy back trigger and detection zone go behind and in front of the enemy when the enemy is looking around
            if (hasEnemyChangedDir == true){
                enemyDetectionZone->position = glm::vec2(enemy->physicsBody->GetPosition().x - 230, enemy->physicsBody->GetPosition().y - 17);
                enemyBack->position = glm::vec2(enemy->physicsBody->GetPosition().x + (enemy->size.x / 2), enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));
            }
            else if (hasEnemyChangedDir == false){
                enemyDetectionZone->position = glm::vec2(enemy->physicsBody->GetPosition().x + (enemy->size.x / 2), enemy->physicsBody->GetPosition().y - 17);
                enemyBack->position = glm::vec2(enemy->physicsBody->GetPosition().x - (enemy->size.x / 2), enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));               
            }

            // make the enemy move when the player is in the detection zone
            if (enemyDetectionZone->isTriggerIntersecting(playerTrigger) && !hasPlayerKilledEnemy){
                enemyState = AGGRO;
            }
        }

        // enemy aggressive state
        if (enemyState == AGGRO){
            // changes the enemy direction change boolean depending on whether x velocity is negative or positive
            if (enemy->physicsBody->GetLinearVelocity().x < 0){
                hasEnemyChangeDirInAggro = false;
            }
            else if (enemy->physicsBody->GetLinearVelocity().x > 0){
                hasEnemyChangeDirInAggro = true;
            }

            // changing the position of the enemy jump and back trigger depending on whether the enemy has changed direction or not
            if (hasEnemyChangeDirInAggro){
                enemyJumpTrigger->position = glm::vec2(enemy->physicsBody->GetPosition().x + (enemy->size.x / 2) - 2, enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));
                enemyBack->position = glm::vec2(enemy->physicsBody->GetPosition().x - (enemy->size.x / 2), enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));
            }
            else {
                enemyJumpTrigger->position = glm::vec2(enemy->physicsBody->GetPosition().x - (enemy->size.x / 2) - 2, enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));                
                enemyBack->position = glm::vec2(enemy->physicsBody->GetPosition().x + (enemy->size.x / 2), enemy->physicsBody->GetPosition().y - (enemy->size.y / 2));               
            }

            // switch to the death scene when the play dies
            if (enemyJumpTrigger->isTriggerIntersecting(playerTrigger)){
                this->state = GAME_DEATH;
            }

            // exclamation mark following the enemy above the enemy's head
            exclamationMark->position = glm::vec2(enemy->physicsBody->GetPosition().x - (enemy->size.x / 2) + 10, enemy->physicsBody->GetPosition().y - 60);

            // For the y direction    
            // if the enemy has collided with an object.
            if (enemyJumpTrigger->isTriggerIntersecting(boxTrigger) && !hasPlayerKilledEnemy && !hasEnemyJumped){
                // apply a jump force
                enemy->physicsBody->ApplyLinearImpulse(b2Vec2(0, -200), enemy->physicsBody->GetWorldCenter(), true);
                hasEnemyJumped = true;
            }
            else{
                hasEnemyJumped = false;
            }
            
            // For the x direction
            // if the enemy is on ground
            if (abs(enemy->physicsBody->GetLinearVelocity().y) > 0.01f && !hasEnemyJumped) {
                if (enemy->physicsBody->GetPosition().x > player->physicsBody->GetPosition().x){
                    // apply a force onto the enemy to the left
                    enemy->physicsBody->ApplyLinearImpulse(b2Vec2(-5, 0), enemy->physicsBody->GetWorldCenter(), true);
                }
                else{
                    // apply a force onto the enemy to the right
                    enemy->physicsBody->ApplyLinearImpulse(b2Vec2(5, 0), enemy->physicsBody->GetWorldCenter(), true);
                }
            }
            
            // or if the enemy is mid-air
            else if (abs(enemy->physicsBody->GetLinearVelocity().y) < 0.01f){
                if (enemy->physicsBody->GetPosition().x > player->physicsBody->GetPosition().x){
                    // apply a force onto the enemy to the left slower 
                    enemy->physicsBody->ApplyLinearImpulse(b2Vec2(-30, 0), enemy->physicsBody->GetWorldCenter(), true);
                }
                else {
                    // apply a force onto the enemy to the right slower
                    enemy->physicsBody->ApplyLinearImpulse(b2Vec2(30, 0), enemy->physicsBody->GetWorldCenter(), true);                    
                }
            }
        }

        // execute the physics process for all physics objects
        player->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
        ground->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
        object->physicsBody->GetWorld()->Step(timeStep, velIterations, posIterations);
    }
}

void GameManager::processInputs(){
    // when the game scene is on game active
    if (this->state == GAME_ACTIVE){
        // this check if the player makes contact with the ground
        bool groundCollision = false; 

        // when the player collides with something it gets put into the contact list, so we iterate over this list
        for (b2ContactEdge* pcd = player->physicsBody->GetContactList(); pcd != NULL; pcd = pcd->next){
            // if the ground physics body is on the contact list then...
            if ((pcd->other == ground->physicsBody || pcd->other == object->physicsBody) && pcd->contact->IsTouching()){
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
        if (this->keys[GLFW_KEY_W] && (abs(playerYVel) <= 0.1) && groundCollision && !isPlayerCrouching){
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
    if (this->state == GAME_ACTIVE){
        // render ground
        ground->draw(*renderer, -1, 0);

        // render text
        testText.Render("This is a test", 0, 0, 10.0f, glm::vec3(0));

        // render player
        if (isPlayerCrouching){
            player->draw(*renderer, 1, 0);
        }
        else{
            player->draw(*renderer, 0, 0);
        }

        // render object 
        object->draw(*renderer, -1, 0);

        // render object trigger
        boxTrigger->draw(*renderer, -1, 0);

        // is the enemy hasn't been murdered
        if (!hasPlayerKilledEnemy){
            if (enemyState == IDLE){
                if (int(glfwGetTime()) % 2 == 0){
                    enemy->draw(*renderer, 0, 0);
                    hasEnemyChangedDir = true;
                }
                if (int(glfwGetTime()) % 2 != 0){
                    enemy->draw(*renderer, 1, 0); 
                    hasEnemyChangedDir = false;
                }

                // render enemyDetectionZone 
                enemyDetectionZone->draw(*renderer, -1, 0);
            }
            
            if (enemyState == AGGRO){
                // rendering all the aggro triggers, the enemy sprites and the exclamation mark.
                if (hasEnemyChangeDirInAggro) enemy->draw(*renderer, 1, 0);
                else enemy->draw(*renderer, 0, 0);
                if (!hasPlayerKilledEnemy){
                    exclamationMark->draw(*renderer, -1, 0);
                }
                enemyBack->draw(*renderer, -1, 0);
            }     
        }
    }
}
