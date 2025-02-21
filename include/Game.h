#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
public:
    Game();
    ~Game();
    void init();
    void update();
    void render(SDL_Renderer* renderer);
    void setMouseCoordinates(int x, int y);
    

private:
    bool gameOver;
};

#endif // GAME_H