#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "GameTable.h"
#include "Boat.h"
#include "EnemyGridShot.h"

class Game {
public:
    Game();

    ~Game();

    GameTable gameTable;

    void init();

    void update() const;

    void render(SDL_Renderer *renderer);

    void setMouseCoordinates(int x, int y);

    void setSquareLocation(int x, int y);

    [[nodiscard]] bool getIsVertical() const;

    void setIsVertical(bool isVertical);

    void setShipSize(int shipSize);

    [[nodiscard]] int getShipSize() const;

    void drawHighlightedShip(SDL_Renderer *renderer);

    void drawShipsList(SDL_Renderer *renderer);

    void drawEnemyShipsList(SDL_Renderer *renderer);

    void setMouseRotateAdjust();

    bool IsCollide(const SDL_Rect &r1, const SDL_Rect &r2);

    bool CheckDirectCollision(const Boat &boat1, const std::vector<Boat> &otherBoats);

    bool CheckPaddingCollision(const Boat &boat1, const std::vector<Boat> &otherBoats);

    bool CheckShipLimits();

    void OnClickSquare();

private:
    std::vector<Boat> ourGridBoats;
    std::vector<EnemyGridShot> enemyGridShots;
    bool gameOver;
    int mouseX;
    int mouseY;
    int indexX;
    int indexY;
    int gridSize = 40;
    int tableSize = 10;
    int _shipSize = 1;
    int placedShipCount[4]{0};
    int placedShipCountLimit[4]{4, 3, 2, 1};
    bool _isVertical = false;

    //SDL_Rect selectedSquare;
    SDL_Rect highlightedSquare{0, 0, 39, 39};

    void SaveSelectedSquare();

    void SaveEnemySquare();

    static bool CheckEnemySquareCollision(const SDL_Rect &newSquare, const std::vector<EnemyGridShot> &enemyFieldShots);
};

#endif // GAME_H