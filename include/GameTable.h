#ifndef SDL2_WATERWAR_GAMETABLE_H
#define SDL2_WATERWAR_GAMETABLE_H

#include <SDL.h>

class GameTable {

public:
    GameTable();
    ~GameTable();
    bool draw(SDL_Renderer *renderer);

private:
    int gridSize = 40;
    int tableSize = 10;
};


#endif //SDL2_WATERWAR_GAMETABLE_H
