#ifndef SDL2_WATERWAR_GAMETABLE_H
#define SDL2_WATERWAR_GAMETABLE_H

#include <SDL.h>

class GameTable {
    int tableSize{10};
    SDL_Rect _rect{0, 0, 40, 40};

public:
    GameTable();

    explicit GameTable(SDL_Rect tableRect);

    ~GameTable();

    bool draw(SDL_Renderer *renderer) const;
};

#endif //SDL2_WATERWAR_GAMETABLE_H
