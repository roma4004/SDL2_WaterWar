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

    void render([[maybe_unused]] SDL_Renderer *renderer);

    void setMouseCoordinates(int x, int y);

    void setSquareLocation(int x, int y);

    [[nodiscard]] bool getIsVertical() const;

    void setIsVertical(bool isVertical);

    void setShipSize(int shipSize);

    [[nodiscard]] int getShipSize() const;

    void drawHighlightedShip(SDL_Renderer *renderer);

    void drawPlayersShips(SDL_Renderer *renderer);

    void drawPlayerShots(SDL_Renderer *renderer);

    void SetRotateAdjust();

    [[nodiscard]] static bool IsCollide(const SDL_Rect &r1, const SDL_Rect &r2);

    [[nodiscard]] static bool IsBoatCollideOtherBoats(const Boat &newBoat, const std::vector<Boat> &otherBoats);

    [[nodiscard]] static bool IsPaddingCollideBoats(const Boat &boat, const std::vector<Boat> &otherBoats);

    [[nodiscard]] bool IsShipLimitReached(bool isPlayerOne);

    void OnClickSquare();

    void OnScrollSquare(const float& preciseY);

    void RotateShip();

private:
    std::vector<Boat> _playerOneGridBoats;//TODO: move to the player class
    std::vector<GridShot> _playerOneGridShots;

    std::vector<Boat> _playerTwoGridBoats;
    std::vector<GridShot> _playerTwoGridShots;

    bool _gameOver;
    bool _isPlayerOneTurn{true};

    int _mouseX;
    int _mouseY;
    int _indexX;
    int _indexY;
    int _gridSize = 40;
    int _tableSize = 10;
    int _shipSize = 1;
    int _placedPlayerOneShipCount[4]{0};
    int _placedPlayerOneShipCountLimit[4]{4, 3, 2, 1};
    int _placedPlayerTwoShipCount[4]{0};
    int _placedPlayerTwoShipCountLimit[4]{4, 3, 2, 1};
    bool _isVertical = false;

    //SDL_Rect selectedSquare;
    SDL_Rect _highlightedBoat{0, 0, 39, 39};

    void SaveShip();

    void SaveShot();

    [[nodiscard]] static bool
    CheckEnemyShotsCollision(const SDL_Rect &newShot, const std::vector<GridShot> &enemyFieldShots);

    [[nodiscard]] static bool IsBoatCollideRect(const Boat &boat, const SDL_Rect &rect);

    [[nodiscard]] static bool IsBoatsCollideRect(const std::vector<Boat> &otherBoats, const SDL_Rect &rect);

    [[nodiscard]] static Boat *ShotCollideOtherBoats(const SDL_Rect &rect, std::vector<Boat> &otherBoats);

    [[nodiscard]] static SDL_Rect GetShipsPadding(const Boat &boat);

    [[nodiscard]] SDL_Rect &CutOutsideGridPlayerOne(SDL_Rect &paddingRect) const;

    [[nodiscard]] SDL_Rect &CutOutsideGridPlayerTwo(SDL_Rect &paddingRect) const;

    [[nodiscard]] static bool IsAllShipPartDamaged(const Boat *damagedBoat);

    [[nodiscard]] SDL_Rect &CutShotOutsideGridPlayer(bool isPlayerOne, SDL_Rect &paddingRect) const;

    [[nodiscard]] bool
    MakeShot(const SDL_Rect &shotRect, const SDL_Rect &damageRect, std::vector <Boat> &playerBoats,
             std::vector <GridShot> &playerShots, bool isPlayerOne);

//    [[nodiscard]]  bool SaveShotPlayerOne(const SDL_Rect &shotRect);

//    [[nodiscard]] bool SaveShotPlayerTwo(const SDL_Rect &shotRect);
    [[nodiscard]] SDL_Rect MapFromShotGridToShipGrid(bool isPlayerOne, int x, int y);
    [[nodiscard]] SDL_Rect MapFromShipGridToShotGrid(bool isPlayerOne, SDL_Rect shipPadding) const;

    [[nodiscard]] SDL_Rect GetShotPadding(bool isPlayerOne, Boat *damagedBoat);
    void scrollShipSize(const float &preciseY);


};

#endif // GAME_H