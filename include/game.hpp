#pragma once

#include <sprite.hpp>
#include <resources.hpp>

enum GameState{
    GAME_ACTIVE, 
    GAME_MENU
};

class GameManager{
    public: 
        GameState state; // game state
        bool keys[1024];
        unsigned int width, height;
        GameManager(unsigned w, unsigned h);
        ~GameManager();
        void init();
        void processInputs(float deltaTime);
        void update(float deltaTime);
        void render();
};