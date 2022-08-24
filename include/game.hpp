#pragma once

// Local Libraries
#include <sprite.hpp>
#include <resources.hpp>

// Game Scenes 
enum GameState{
    GAME_ACTIVE, 
    GAME_MENU
};

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