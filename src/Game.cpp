#include "Game.h"
#include <iostream>
#include <SDL.h>

using namespace std;

Game::Game() : gameOver(false) {}

Game::~Game() {}

void Game::init() {
   
}

void Game::update() {
    if (gameOver) return;
    
    
}

void Game::render(SDL_Renderer* renderer) {
   
}

