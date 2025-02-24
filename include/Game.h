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
    void setSquareLocation(int x, int y);
    void drawSelectedSquare(SDL_Renderer* renderer);
    int getGridSize();

private:
    bool gameOver;
    int mouseX;
    int mouseY;
    int indexX;
    int indexY;
    int gridSize = 40;
    int tableSize = 10;
    //SDL_Rect selectedSquare;
    SDL_Rect highlightedSquare{0,0, 38,38};

};

#endif // GAME_H