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

void Game::setMouseRotateAdjust()
{
    this->indexX = this->mouseX / this->gridSize;
    this->indexY = this->mouseY / this->gridSize;
    if (_isVertical) {
        if (this->indexY + _shipSize > this->tableSize) {
            this->indexY += this->tableSize - (this->indexY + _shipSize);
        }
    }
    else
    {
        if (this->indexX + _shipSize > this->tableSize) {
            this->indexX += this->tableSize - (this->indexX + _shipSize);
        }
    }

    cout << "rotate: " << "x: " << this->indexX << " y: " << this->indexY << endl;
}

void Game::setMouseCoordinates(int x, int y) {
    this->mouseX = x;
    this->mouseY = y;
    cout << "Event type: " << "x: " << this->mouseX << " y: " << this->mouseY << endl;
}

void Game::setSquareLocation(int x, int y){
    this->indexX = x / this->gridSize;
    this->indexY = y / this->gridSize;
    if (_isVertical) {
        if (this->indexY + _shipSize > this->tableSize) {
            this->indexY = 10 - _shipSize;
            return;
        }
    }
    else
    {
        if (this->indexX + _shipSize > this->tableSize) {
            this->indexX = 10 - _shipSize;
            return;
        }
    }


    cout << "Square location: " << "x: " << this->indexX << " y: " << this->indexY << endl;
}

void Game::saveSelectedSquare() {
    // confirm validation
    this->boats.emplace_back(this->highlightedSquare, getShipSize(), getIsVertical());
}

void Game::drawSquareList(SDL_Renderer *renderer) {
    for (const auto& boat : this->boats) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (const auto& boatPart : boat.body) {
            const auto rect = boatPart.getRect();
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Game::drawHighlightedSquare(SDL_Renderer *renderer) {
    if (this->indexX > this->tableSize - 1 || this->indexY > this->tableSize - 1)
        return;

    this->highlightedSquare.x = this->indexX * this->gridSize + 1;
    this->highlightedSquare.y = this->indexY * this->gridSize + 1;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    auto hRect = this->highlightedSquare;
    const int size = getShipSize();
    const bool isVertical = getIsVertical();
    SDL_Rect rect;
    for (int i = 0; i < size; i++) {
        if (isVertical)
        {
            rect = SDL_Rect{hRect.x, hRect.y + (hRect.h + 1) * i, hRect.w, hRect.h };
        }
        else
        {
            rect = SDL_Rect{hRect.x + (hRect.w + 1) * i, hRect.y, hRect.w, hRect.h};
        }

        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::render(SDL_Renderer* renderer) {}

void Game::setIsVertical(bool isVertical) {
    _isVertical = isVertical;
}

void Game::setShipSize(int shipSize) {
    _shipSize = shipSize;
}

bool Game::getIsVertical() {
    return _isVertical;
}

int Game::getShipSize() {
    return _shipSize;
}

