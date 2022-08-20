#pragma once

#include <spriterenderer.hpp>
#include <resourcemanager.hpp>

// represent the states of the game
enum GameState {
    GAME_ACTIVE, 
    GAME_MENU
};

class GameManager{
    public: 
        GameState state; // game state
        bool keys[1024]; 
        unsigned int width, height;
        GameManager(unsigned w, unsigned h); // constructor
        ~GameManager(); // destructor
        void init(); // initializer 
        // game loop
        void processInputs(float deltaTime);
        void update(float deltaTime);
        void render();
};