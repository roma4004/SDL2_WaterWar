#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "GameTable.h"
#include "Boat.h"
#include "GridShot.h"

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

    void drawPlayerShips(SDL_Renderer *renderer);

    void drawPlayerShots(SDL_Renderer *renderer);

    void setRotateAdjust();

    [[nodiscard]] static bool IsCollide(const SDL_Rect &r1, const SDL_Rect &r2);

    [[nodiscard]] static bool IsBoatCollideOtherBoats(const Boat &newBoat, const std::vector<Boat> &otherBoats);

    [[nodiscard]] static bool IsBoatsCollidePaddingRect(const Boat &boat, const std::vector<Boat> &otherBoats) ;

    [[nodiscard]] bool IsShipLimitReached();

    void OnClickSquare();

private:
    std::vector<Boat> _playerGridBoats;//TODO: move to the player class
    std::vector<GridShot> _playerGridShots;
    bool _gameOver;
    int _mouseX;
    int _mouseY;
    int _indexX;
    int _indexY;
    int _gridSize = 40;
    int _tableSize = 10;
    int _shipSize = 1;
    int _placedShipCount[4]{0};
    int _placedShipCountLimit[4]{4, 3, 2, 1};
    bool _isVertical = false;

    //SDL_Rect selectedSquare;
    SDL_Rect _highlightedBoat{0, 0, 39, 39};

    void SaveBoat();

    void SaveShot();

    [[nodiscard]] static bool CheckEnemySquareCollision(const SDL_Rect &shot, const std::vector<GridShot> &enemyFieldShots);

    [[nodiscard]] static bool IsBoatCollideRect(const Boat &boat, const SDL_Rect &rect);

    [[nodiscard]] static bool IsBoatsCollideRect(const std::vector<Boat> &otherBoats, const SDL_Rect &rect);

    [[nodiscard]] static SDL_Rect CalkShipsPadding(const Boat &boat);
};

#endif // GAME_H