#pragma once

// Local Libraries
#include <sprite.hpp>
#include <resources.hpp>
#include <gameobject.hpp>
#include <texture2D.hpp>

// GLFW library
#include <glfw/glfw3.h>

// Box2D library
#include <box2d/box2d.h>
#include <box2d/b2_world.h>

// Game Scenes 
enum GameState{
    GAME_ACTIVE, 
    GAME_MENU
};

// Player constants 
const float PLAYER_VELOCITY = 10.0f;

class GameManager{
    public: 
        GameState state; // game state
        bool keys[1024]; // key inputs
        unsigned int width, height; // dimensions of projection
        GameManager(unsigned w, unsigned h); // constructor
        ~GameManager(); // destructor
        void init(); // initialization function before game loop
        void processInputs(float deltaTime); // input handling during game loop
        void update(float deltaTime); // dynamics movement of gameObjects during game loop
        void render(); // rendering operations 
};