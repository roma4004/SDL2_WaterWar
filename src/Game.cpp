#include "Game.h"
#include <iostream>
#include <SDL.h>

using namespace std;

Game::Game() : gameOver(false), mouseX(0), mouseY(0) {}

Game::~Game() {}

void Game::init() {
   
}

void Game::update() {
    if (gameOver) return;
    
    
}

void Game::render(SDL_Renderer* renderer) {
   
}

void Game::setMouseCoordinates(int x, int y) {
    this->mouseX = x;
    this->mouseY = y;
    cout << "Event type: " << "x: " << this->mouseX << " y: " << this->mouseY << endl;
}

void Game::setSquareLocation(int x, int y){
    this->squareX = x/40;
    this->squareY = y/40;

    cout << "Square location: " << "x: " << this->squareX << " y: " << this->squareY << endl;
}

