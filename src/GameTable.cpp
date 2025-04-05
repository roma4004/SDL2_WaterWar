#include "../include/GameTable.h"

GameTable::GameTable() = default;

GameTable::GameTable(SDL_Rect tableRect) : _rect{tableRect} {}

GameTable::~GameTable() = default;

bool GameTable::draw(SDL_Renderer *renderer) const {
    // Draw the grid
    int size = _rect.w * (tableSize);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (int y = _rect.y; y <= tableSize + _rect.y; ++y) {
        for (int x = _rect.x; x <= tableSize + _rect.x; ++x) {
            SDL_RenderDrawLine(renderer, x * _rect.w, y, x * _rect.w, size); // vertical line
        }
        SDL_RenderDrawLine(renderer, _rect.x * _rect.w, y * _rect.h,
                           _rect.x * _rect.w + size, y * _rect.h); // horizontal line
    }

    return true;
}

