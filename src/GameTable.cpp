#include "../include/GameTable.h"

GameTable::GameTable() = default;

GameTable::GameTable(SDL_Rect tableRect) : _rect{tableRect} {}

GameTable::~GameTable() = default;

bool GameTable::draw(SDL_Renderer *renderer) const {
    // Draw the grid
    int lineSizeX = _rect.w * (tableSize);
    int lineSizeY = _rect.h * (tableSize);
    int xOffset = _rect.x;
    int yOffset = _rect.y;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

    for (int y = yOffset; y <= tableSize + yOffset; ++y) {
        for (int x = xOffset; x <= tableSize + xOffset; ++x) {
            SDL_RenderDrawLine(renderer,
                               x * _rect.w, y * _rect.h,
                               x * _rect.w, yOffset * _rect.h + lineSizeY); // vertical line
        }

        SDL_RenderDrawLine(renderer,
                           xOffset * _rect.w, y * _rect.h,
                           xOffset * _rect.w + lineSizeX, y * _rect.h); // horizontal line
    }

    return true;
}
