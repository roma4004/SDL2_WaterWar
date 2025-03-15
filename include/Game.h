#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "GameTable.h"
#include "Boat.h"

class Game {
public:
    Game();
    ~Game();
    GameTable gameTable;
    void init();
    void update();
    void render(SDL_Renderer* renderer);
    void setMouseCoordinates(int x, int y);
    void setSquareLocation(int x, int y);
    bool getIsVertical();
    void setIsVertical(bool isVertical);
    void setShipSize(int shipSize);
    int getShipSize();
    void drawHighlightedSquare(SDL_Renderer* renderer);
    void saveSelectedSquare();
    void drawSquareList(SDL_Renderer* renderer);

private:
    std::vector<SDL_Rect> listOfCubes;
    bool gameOver;
    int mouseX;
    int mouseY;
    int indexX;
    int indexY;
    int gridSize = 40;
    int tableSize = 10;
    //SDL_Rect selectedSquare;
    SDL_Rect highlightedSquare{0, 0, 39, 39};

};

#endif // GAME_H