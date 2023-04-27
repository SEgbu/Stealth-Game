#include <game.hpp>
#include <noncollidable.hpp>
#include <trigger.hpp>
#include <vector>
#include <button.hpp>
#include <text.hpp>
#include <string>
#include <SDL2/SDL_mixer.h>

// The game renderer
SpriteRenderer* renderer;

// Music variables
Mix_Music* mainGame; 
Mix_Music* menuGame;

// The objects
GameObject* player;
GameObject* ground;
GameObject* object;
GameObject* enemy;

Trigger* enemyDetectionZone;
Trigger* enemyBack;
Trigger* playerTrigger;
Trigger* enemyJumpTrigger;
Trigger* boxTrigger;
Trigger* diamond;

NonCollidableObject* exclamationMark;
NonCollidableObject* deathScreenText;
NonCollidableObject* logo;
NonCollidableObject* playerMenu;
NonCollidableObject* enemyMenu;
NonCollidableObject* winScreenText;

Button* menuDeathScreenButton;
Button* quitDeathScreenButton;
Button* restartDeathScreenButton; 

Button* playMenuScreenButton;
Button* quitMenuScreenButton;

Button* menuWinScreenButton;
Button* quitWinScreenButton;

Text* scoreText;
Text* deathScreenHighScore;

// enumerator for enemy state
enum EnemyState {
    IDLE, 
    AGGRO
};

// enemy default state is idle
EnemyState enemyState = IDLE;

// player crouching boolean
bool isPlayerCrouching = false;

// a boolean which checks if music is playing 
bool isMusicPlaying = false;

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

// a boolean which detects whether the game has been initialized
bool hasInit = false;

// file writer and reader of memory
std::ofstream outHighScoreFile;
std::ifstream inHighScoreFile;
std::stringstream highScoreBuffer;
std::string getterHighScoreString;

// integers for the scores
int temp = 0;
int* currentScore = &(temp); 
int* highScore = new int;

// rendering the highscore on the death scree
std::string deathScreenHighScoreString;

// Sets all public attributes to something
GameManager::GameManager(unsigned int w, unsigned int h) : width(w), height(h), state(GAME_MENU), quitProgram(false){

}

// deallocates renderer 
GameManager::~GameManager(){
    // deleting render 
    delete renderer;

    // clearing the music variables
    Mix_FreeMusic(mainGame);
    mainGame = NULL;

    Mix_FreeMusic(menuGame);
    menuGame = NULL;
    
    // deleting game objects
    delete player;
    delete ground;
    delete object;
    delete enemy;

    // deleting triggers
    delete enemyDetectionZone;
    delete enemyBack; 
    delete playerTrigger;
    delete enemyJumpTrigger;
    delete boxTrigger;
    delete diamond;

    // deleting images
    delete exclamationMark;
    delete deathScreenText;

    delete logo;
    delete playerMenu;
    delete enemyMenu;

    // deleting buttons
    delete menuDeathScreenButton;
    delete quitDeathScreenButton;
    delete restartDeathScreenButton; 
    
    delete playMenuScreenButton;
    delete quitMenuScreenButton;

    // deleting text
    delete scoreText;
    delete deathScreenHighScore;
    
    // get score and close scoreFile
    if (*currentScore > *highScore){
        outHighScoreFile.open("save/score.txt", std::ios::out | std::ios::trunc);

        if (outHighScoreFile.is_open()){
            outHighScoreFile << *currentScore;
            
            // if is fail print an error
            if (outHighScoreFile.fail()){
                std::cout << "Error" << std::endl;
            }
        }

        outHighScoreFile.close();
    }
    else {
        outHighScoreFile.open("save/score.txt", std::ios::out | std::ios::trunc);

        if (outHighScoreFile.is_open()){
            outHighScoreFile << *highScore;
            
            // if is fail print an error
            if (outHighScoreFile.fail()){
                std::cout << "Error" << std::endl;
            }
        }

        outHighScoreFile.close();
    }
}

// initialize variables 
void GameManager::init(){
    if (!hasInit){
        // setting up shader program
        ResourceManager::loadShader("shaders/vertex.vs", "shaders/fragment.fs", "sprite"); 

        // setting up projection matrix for the player viewing the game 
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

        // setting up the music variables 
        mainGame = Mix_LoadMUS("assets/stealth game main game.wav");
        if (mainGame == nullptr){
            std::cout << "Failed to load the main game music, SDL mixer error: " << Mix_GetError() << std::endl;
        }

        menuGame = Mix_LoadMUS("assets/stealth game menu.wav");
        if (menuGame == nullptr){
            std::cout << "Failed to load the menu game music, SDL mixer error: " << Mix_GetError() << std::endl;
        }

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

        // loading and generating the exclamation mark texture
        ResourceManager::loadTexture("assets/exclamation.png", true, "exclamation");

        // loading and generating the death screen text texture
        ResourceManager::loadTexture("assets/you died.png", true, "deathScreenText");

        // loading and generating the diamond texture
        ResourceManager::loadTexture("assets/diamond.png", true, "diamond");

        // loading and generating the button textures
        ResourceManager::loadTexture("assets/menuIdleDeathScreenButton.png", true, "menuDeathButtonIdle");
        ResourceManager::loadTexture("assets/menuHoverDeathScreenButton.png", true, "menuDeathButtonHover");
        ResourceManager::loadTexture("assets/menuPressedDeathScreenButton.png", true, "menuDeathButtonPressed");

        ResourceManager::loadTexture("assets/quitIdleDeathScreenButton.png", true, "quitDeathButtonIdle");
        ResourceManager::loadTexture("assets/quitHoverDeathScreenButton.png", true, "quitDeathButtonHover");
        ResourceManager::loadTexture("assets/quitPressedDeathScreenButton.png", true, "quitDeathButtonPressed");

        ResourceManager::loadTexture("assets/restartIdleDeathScreenButton.png", true, "restartDeathButtonIdle");
        ResourceManager::loadTexture("assets/restartHoverDeathScreenButton.png", true, "restartDeathButtonHover");
        ResourceManager::loadTexture("assets/restartPressedDeathScreenButton.png", true, "restartDeathButtonPressed");

        // loading and generating main menu buttons and images
        ResourceManager::loadTexture("assets/playIdleMenuScreenButton.png", true, "playMenuButtonIdle");
        ResourceManager::loadTexture("assets/playHoverMenuScreenButton.png", true, "playMenuButtonHover");
        ResourceManager::loadTexture("assets/playPressedMenuScreenButton.png", true, "playMenuButtonPressed");

        ResourceManager::loadTexture("assets/guardMenu.png", true, "enemyMenuImage");
        ResourceManager::loadTexture("assets/playerMenu.png", true, "playerMenuImage");
        ResourceManager::loadTexture("assets/gameLogo.png", true, "gameLogoImage");

        // loading and generating win screen text image
        ResourceManager::loadTexture("assets/youWin.png", true, "winScreenText");

    }

    // list of play menu button textures
    std::vector<Texture2D> playMenuButtonTextures = {ResourceManager::getTexture("playMenuButtonIdle"), ResourceManager::getTexture("playMenuButtonHover"), ResourceManager::getTexture("playMenuButtonPressed")};

    // list of player textures
    std::vector<Texture2D> playerTextures = {ResourceManager::getTexture("player"), ResourceManager::getTexture("playerCrouch")};

    // list of enemy textures
    std::vector<Texture2D> enemyTextures = {ResourceManager::getTexture("enemyLeft"), ResourceManager::getTexture("enemyRight")};

    // list of test button textures
    std::vector<Texture2D> menuDeathButtonTextures = {ResourceManager::getTexture("menuDeathButtonIdle"), ResourceManager::getTexture("menuDeathButtonHover"), ResourceManager::getTexture("menuDeathButtonPressed")};
    std::vector<Texture2D> quitDeathButtonTextures = {ResourceManager::getTexture("quitDeathButtonIdle"), ResourceManager::getTexture("quitDeathButtonHover"), ResourceManager::getTexture("quitDeathButtonPressed")};
    std::vector<Texture2D> restartDeathButtonTextures = {ResourceManager::getTexture("restartDeathButtonIdle"), ResourceManager::getTexture("restartDeathButtonHover"), ResourceManager::getTexture("restartDeathButtonPressed")};
    
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

    // creating the death screen text
    deathScreenText = new NonCollidableObject(glm::vec2((this->width / 2) - (600 / 2), 0), glm::vec2(600, (600 / 16) * 9), ResourceManager::getTexture("deathScreenText"));

    // creating the score text
    scoreText = new Text(glm::vec2(0, 0), glm::vec2(30));

    // creating the death screen high score text
    deathScreenHighScore = new Text(glm::vec2 ((this->width / 2) - 175, 150), glm::vec2(30));

    // loading highscore from file and setting it to high score
    if (hasInit == false){
        inHighScoreFile.open("save/score.txt");

        hasInit = true;
        if (inHighScoreFile.is_open()){
            highScoreBuffer << inHighScoreFile.rdbuf();
            getterHighScoreString = highScoreBuffer.str();

            if (getterHighScoreString.length() == 0)
                *highScore = 0;
            else
                *highScore = std::stoi(getterHighScoreString);
        }

        inHighScoreFile.close();
    }



    // creating death screen buttons
    menuDeathScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 220), glm::vec2(200, 113), menuDeathButtonTextures);
    quitDeathScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 480), glm::vec2(200, 113), quitDeathButtonTextures);
    restartDeathScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 350), glm::vec2(200, 113), restartDeathButtonTextures);
    
    // initializing death screen buttons
    menuDeathScreenButton->init();
    quitDeathScreenButton->init();
    restartDeathScreenButton->init();

    // main menu items
    enemyMenu = new NonCollidableObject(glm::vec2(30, 100), glm::vec2(200, 250), ResourceManager::getTexture("enemyMenuImage"));
    playerMenu = new NonCollidableObject(glm::vec2(500, 100), glm::vec2(225), ResourceManager::getTexture("playerMenuImage"));
    logo = new NonCollidableObject(glm::vec2(this->width / 2 - 200, 50), glm::vec2(400, 300), ResourceManager::getTexture("gameLogoImage"));

    playMenuScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 350), glm::vec2(200, 113), playMenuButtonTextures);
    quitMenuScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 480), glm::vec2(200, 113), quitDeathButtonTextures);

    // initializing menu screen buttons
    playMenuScreenButton->init();
    quitMenuScreenButton->init();

    // Creating the diamond object 
    diamond = new Trigger(glm::vec2(640, 480), glm::vec2(35, 35), ResourceManager::getTexture("diamond"));

    // Creating the win screen text
    winScreenText = new NonCollidableObject(glm::vec2(this->width / 2 - 200, 50), glm::vec2(300, 100), ResourceManager::getTexture("winScreenText"));

    // Creating the win screen buttons 
    menuWinScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 220), glm::vec2(200, 113), menuDeathButtonTextures);
    quitWinScreenButton = new Button(glm::vec2((this->width / 2) - (200 / 2), 480), glm::vec2(200, 113), quitDeathButtonTextures);

    // set time to zero 
    glfwSetTime(time);
}

void GameManager::update(){
    if (this->state == GAME_ACTIVE){
        if (!isMusicPlaying){        
            Mix_PlayMusic(mainGame, -1);
            isMusicPlaying = true;
        }

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
            *currentScore += 5;
        }

        if (player->physicsBody->GetPosition().y > this->height + 20){
            this->state = GAME_DEATH;
        }

        // diamond float
        diamond->position.y = 480 + 15*sin(2*glfwGetTime());

        // winning the game 
        if (diamond->isTriggerIntersecting(playerTrigger)){
            this->state = GAME_WIN;
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
    else if (this->state == GAME_DEATH){
        Mix_HaltMusic();
        isMusicPlaying = false;

        *currentScore = 0;
        if (quitDeathScreenButton->isPressed){
            this->quitProgram = true;
        }
        if (restartDeathScreenButton->isPressed){
            this->state = GAME_ACTIVE;
            this->init();
            enemyState = EnemyState::IDLE;
            hasPlayerKilledEnemy = false;
        }
        if (menuDeathScreenButton->isPressed){
            this->state = GAME_MENU;
        }
    }
    else if (this->state == GAME_MENU){
        if (!isMusicPlaying){
            Mix_PlayMusic(menuGame, -1);
            isMusicPlaying = true;
        }      

        *currentScore = 0;
        if (playMenuScreenButton->isPressed){
            this->state = GAME_ACTIVE;
            enemyState = EnemyState::IDLE;
            hasPlayerKilledEnemy = false;
            isMusicPlaying = false;
        }
        if (quitMenuScreenButton->isPressed){
            this->quitProgram = true;
            isMusicPlaying = false;
        }
    }
    else if (this->state == GAME_WIN){
        Mix_HaltMusic();
        isMusicPlaying = false;

        if (menuWinScreenButton->isPressed){
            this->init();
            playMenuScreenButton->isPressed = false;
            quitMenuScreenButton->isPressed = false;
            this->state = GAME_MENU;
            this->quitProgram = false;
        }
        if (quitWinScreenButton->isPressed){
            this->quitProgram = true;
            isMusicPlaying = false;
        }
    }
}

void GameManager::processInputs(){
    playMenuScreenButton->init();
    quitMenuScreenButton->init();

    menuDeathScreenButton->init();
    restartDeathScreenButton->init();
    quitDeathScreenButton->init();

    menuWinScreenButton->init();
    quitWinScreenButton->init();

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
        if ((this->keys[GLFW_KEY_W] || this->keys[GLFW_KEY_UP]) && (abs(playerYVel) <= 0.1) && groundCollision && !isPlayerCrouching){
            player->physicsBody->ApplyLinearImpulse(b2Vec2(0, -10000.0f), player->physicsBody->GetWorldCenter(), true);
        }

        // sets the crouching boolean true if the s key is pressed
        if (this->keys[GLFW_KEY_S] || this->keys[GLFW_KEY_DOWN]){
            isPlayerCrouching = true;
        }
        else {
            isPlayerCrouching = false;
        }
    }
    else if (this->state == GAME_DEATH){
        menuDeathScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
        quitDeathScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
        restartDeathScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
    }
    else if (this->state == GAME_MENU){
        playMenuScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
        quitMenuScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
    }
    else if (this->state == GAME_WIN){
        menuWinScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
        quitWinScreenButton->eventHandler(this->mousePos.xPos, this->mousePos.yPos, this->leftClick);
    }
}

void GameManager::render(){
    deathScreenHighScoreString = "HIGHSCORE: "+std::to_string(*highScore);

    if (this->state == GAME_ACTIVE){
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

        // render score
        scoreText->draw(*renderer, "SCORE: "+std::to_string(*currentScore));

        // rendering the diamond trigger
        diamond->draw(*renderer, -1, 0);

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
    else if (this->state == GAME_DEATH){
        deathScreenText->draw(*renderer, -1, 0);

        if (*currentScore > *highScore){
            // updating the new highscore
            *highScore = *currentScore;
        }
        
        deathScreenHighScore->draw(*renderer, deathScreenHighScoreString);


        if (menuDeathScreenButton->isHover && !menuDeathScreenButton->isPressed){
            menuDeathScreenButton->draw(*renderer, 1, 0);
        }
        else if (!menuDeathScreenButton->isHover && menuDeathScreenButton->isPressed){
            menuDeathScreenButton->draw(*renderer, 2, 0);
        }
        else {
            menuDeathScreenButton->draw(*renderer, 0, 0);
        }
        
        if (quitDeathScreenButton->isHover && !quitDeathScreenButton->isPressed){
            quitDeathScreenButton->draw(*renderer, 1, 0);
        }
        else if (!quitDeathScreenButton->isHover && quitDeathScreenButton->isPressed){
            quitDeathScreenButton->draw(*renderer, 2, 0);
        }
        else {
            quitDeathScreenButton->draw(*renderer, 0, 0);
        }

        if (restartDeathScreenButton->isHover && !restartDeathScreenButton->isPressed){
            restartDeathScreenButton->draw(*renderer, 1, 0);
        }
        else if (!restartDeathScreenButton->isHover && restartDeathScreenButton->isPressed){
            restartDeathScreenButton->draw(*renderer, 2, 0);        }
        else {
            restartDeathScreenButton->draw(*renderer, 0, 0);
        }
    }
    else if (this->state == GAME_MENU){
        enemyMenu->draw(*renderer, -1, 0);
        playerMenu->draw(*renderer, -1, 0);
        logo->draw(*renderer, -1, 0);

        if (quitMenuScreenButton->isHover && !quitMenuScreenButton->isPressed){
            quitMenuScreenButton->draw(*renderer, 1, 0);
        }
        else if (!quitMenuScreenButton->isHover && quitMenuScreenButton->isPressed){
            quitMenuScreenButton->draw(*renderer, 2, 0);
        }
        else {
            quitMenuScreenButton->draw(*renderer, 0, 0);
        }

        if (playMenuScreenButton->isHover && !playMenuScreenButton->isPressed){
            playMenuScreenButton->draw(*renderer, 1, 0);
        }
        else if (!playMenuScreenButton->isHover && playMenuScreenButton->isPressed){
            playMenuScreenButton->draw(*renderer, 2, 0);
        }
        else {
            playMenuScreenButton->draw(*renderer, 0, 0);
        }   
    }
    else if (this->state == GAME_WIN){
        deathScreenHighScore->draw(*renderer, deathScreenHighScoreString);
        
        winScreenText->draw(*renderer, -1, 0);

        if (quitWinScreenButton->isHover && !quitWinScreenButton->isPressed){
            quitWinScreenButton->draw(*renderer, 1, 0);
        }
        else if (!quitWinScreenButton->isHover && quitWinScreenButton->isPressed){
            quitWinScreenButton->draw(*renderer, 2, 0);
        }
        else {
            quitWinScreenButton->draw(*renderer, 0, 0);
        }   

        if (menuWinScreenButton->isHover && !menuWinScreenButton->isPressed){
            menuWinScreenButton->draw(*renderer, 1, 0);
        }
        else if (!menuWinScreenButton->isHover && menuWinScreenButton->isPressed){
            menuWinScreenButton->draw(*renderer, 2, 0);
        }
        else {
            menuWinScreenButton->draw(*renderer, 0, 0);
        }   
    }
}
