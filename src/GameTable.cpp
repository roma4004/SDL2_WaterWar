#include "../include/GameTable.h"

GameTable::GameTable() = default;

GameTable::~GameTable() = default;

bool GameTable::draw(SDL_Renderer *renderer) const {
    // Draw the grid
    int size = gridSize * (tableSize);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (int y = 0; y <= tableSize; y++) {
        for (int x = 0; x <= tableSize; x++) {
            SDL_RenderDrawLine(renderer, x * gridSize, 0, x * gridSize, size);
        }
        SDL_RenderDrawLine(renderer, 0, y * gridSize, size, y * gridSize);
    }
    return true;
}

