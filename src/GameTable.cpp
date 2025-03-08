#include "../include/GameTable.h"

GameTable::GameTable()
{
}

GameTable::~GameTable()
{
}

bool GameTable::draw(SDL_Renderer *renderer) {
    // Draw the grid
    int size = gridSize * (tableSize - 1);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (int y = 0; y < tableSize; y++) {
        for (int x = 0; x < tableSize; x++){
            SDL_RenderDrawLine(renderer, x * gridSize, 0, x * gridSize, size);
        }
        SDL_RenderDrawLine(renderer, 0, y * gridSize, size, y * gridSize);
    }
    return true;
}

