#include "Game.h"
#include <iostream>
#include <SDL.h>

using namespace std;

Game::Game() : gameOver(false), mouseX(0), mouseY(0), indexX(0), indexY(0) {}

Game::~Game() {}

void Game::init() {}

void Game::update() {
    if (gameOver) return;
    
}

void Game::setMouseCoordinates(int x, int y) {
    this->mouseX = x;
    this->mouseY = y;
    cout << "Event type: " << "x: " << this->mouseX << " y: " << this->mouseY << endl;
}

void Game::setSquareLocation(int x, int y){

    this->indexX = x / this->gridSize;
    this->indexY = y / this->gridSize;

    cout << "Square location: " << "x: " << this->indexX << " y: " << this->indexY << endl;
}

void Game::drawSelectedSquare(SDL_Renderer *renderer) {
    if (this->indexX > this->tableSize - 1 || this->indexY > this->tableSize - 1)
        return;

    this->highlightedSquare.x = this->indexX * this->gridSize + 1;
    this->highlightedSquare.y = this->indexY * this->gridSize + 1;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &this->highlightedSquare);
}

void Game::render(SDL_Renderer* renderer) {}

