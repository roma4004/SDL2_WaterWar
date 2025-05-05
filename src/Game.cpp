#include "Game.h"
#include "GridShot.h"

#include <iostream>
#include <SDL.h>
#include <algorithm>

using namespace std;

Game::Game() = default;

Game::~Game() = default;

void Game::init() {}

void Game::update() const {
    if (_gameOver) return;
}

const bool Game::GetGameMode() {return _gameMode;}

const bool Game::IsAllShipsPlaced() {
    for (int i = 0; i < 4; i++) {
        if (_placedPlayerOneShipCount[i] != _placedPlayerOneShipCountLimit[i]
            || _placedPlayerTwoShipCount[i] != _placedPlayerTwoShipCountLimit[i]) {
            return false;
        }
    }
    return true;
};

void Game::ChangeGameMode() {
    if (IsAllShipsPlaced()) {
        _gameMode = true;
    }
    else {
        _gameMode = false;
    }
}

void Game::SetRotateAdjust() {
    _indexX = _mouseX / _gridSize;
    _indexY = _mouseY / _gridSize;

    constexpr int gridPadding = 2; //TODO: should be class member
    if (_indexX >= _tableSize //right side after one grids
        || (_indexY >= _tableSize && _indexY < _tableSize + gridPadding) //vertical grid padding
        || _indexY >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    if (_isVertical) {
        const int shipHead = _indexY;
        const int shipTail = _indexY + _shipSize;
        if ((shipTail > _tableSize && shipTail <= _tableSize + gridPadding) //vertical grid padding
            || shipTail > _tableSize * 2 + gridPadding //bottom side after two grids
            || (shipHead == _tableSize - 1 && shipTail > _tableSize + gridPadding)) {//case when tail in different grid

            if (shipHead > _tableSize + gridPadding) {
                _indexY = _tableSize * 2 + gridPadding - _shipSize;
            } else {
                _indexY = _tableSize - _shipSize;
            }
        }
    } else {
        const int shipTail = _indexX + _shipSize;
        if (shipTail > _tableSize) {
            _indexX = _tableSize - _shipSize;
        }
    }

    cout << "adjusted to: " << "x: " << _indexX << " y: " << _indexY << endl;
}

void Game::setMouseCoordinates(int x, int y) {
    _mouseX = x;
    _mouseY = y;
//    cout << "setMouseCoordinates x: " << _mouseX << " y: " << _mouseY << endl;
}

void Game::setSquareLocation(int x, int y) {
    _indexX = x / _gridSize;
    _indexY = y / _gridSize;

    constexpr int gridPadding = 2;
    if (_indexX >= _tableSize //right side after one grids
        || (_indexY >= _tableSize && _indexY < _tableSize + gridPadding) //vertical grid padding
        || _indexY >= _tableSize * 2 + gridPadding) {  //bottom side after grids
        return;
    }

    if (_isVertical) {
        const int shipHead = _indexY;
        const int shipTail = _indexY + _shipSize;
        if ((shipTail > _tableSize &&
             shipTail <= _tableSize + gridPadding) //vertical grid padding
            || shipTail > _tableSize * 2 + gridPadding//bottom side after grids
            || (shipHead == _tableSize - 1 &&
                shipTail > _tableSize + gridPadding)) { //case when tail in different grid

            if (shipHead > _tableSize + gridPadding) {
                _indexY = _tableSize * 2 + gridPadding - _shipSize;
            } else {
                _indexY = _tableSize - _shipSize;
            }

            return;
        }
    } else {
        const int shipTail = _indexX + _shipSize;
        if (shipTail > _tableSize) {
            _indexX = _tableSize - _shipSize;
            return;
        }
    }


    cout << "Square location: " << "x: " << _indexX << " y: " << _indexY << endl;
}

bool Game::IsCollide(const SDL_Rect &rect1, const SDL_Rect &rect2) {
    const bool isSeparatedHorizontally = rect1.x + rect1.w <= rect2.x || rect2.x + rect2.w <= rect1.x;
    const bool isSeparatedVertically = rect1.y + rect1.h <= rect2.y || rect2.y + rect2.h <= rect1.y;

    // Return true if the rectangles overlap (not separated horizontally or vertically)
    if (isSeparatedHorizontally || isSeparatedVertically) {
        return false;
    }

    return true;
}

bool Game::CheckEnemyShotsCollision(const SDL_Rect &newShot, const std::vector<GridShot> &enemyFieldShots) {
    return std::none_of(enemyFieldShots.begin(), enemyFieldShots.end(), [&newShot](const GridShot &shot) {
        const auto &oldShot = shot.getRect();
        return newShot.x == oldShot.x && newShot.y == oldShot.y;
    });
}

bool Game::IsPaddingCollideBoats(const Boat &boat, const std::vector<Boat> &otherBoats) {
    const auto paddingRect = GetShipsPadding(boat);
    if (IsBoatsCollideRect(otherBoats, paddingRect)) {
        return true;
    }

    return false;
}

bool Game::IsBoatsCollideRect(const std::vector<Boat> &otherBoats, const SDL_Rect &rect) {
    for (const auto &boat: otherBoats) {
        if (IsBoatCollideRect(boat, rect)) {
            return true;
        }
    }

    return false;
}

bool Game::IsBoatCollideRect(const Boat &boat, const SDL_Rect &rect) {
    for (const auto &boatPart: boat.body) {
        if (IsCollide(rect, boatPart.getRect())) {
            return true;
        }
    }

    return false;
}


bool Game::IsBoatCollideOtherBoats(const Boat &newBoat, const std::vector<Boat> &otherBoats) {
    for (const auto &newBoatPart: newBoat.body) {
        const auto rect = newBoatPart.getRect();
        if (IsBoatsCollideRect(otherBoats, rect)) {
            return true;
        }
    }

    return false;
}

Boat *Game::ShotCollideOtherBoats(const SDL_Rect &rect, std::vector<Boat> &otherBoats) {
    for (auto &boat: otherBoats) {
        for (auto &boatPart: boat.body) {
            if (IsCollide(rect, boatPart.getRect())) {
                boatPart._color = 0xffff00;

                return &boat;
            }
        }
    }

    return nullptr;
}

bool Game::IsShipLimitReached(bool isPlayerOne) { //TODO: rename boat to ship
    const int shipSizeToIndex = getShipSize() - 1;
    if (isPlayerOne) {
        if (_placedPlayerOneShipCount[shipSizeToIndex] < _placedPlayerOneShipCountLimit[shipSizeToIndex]) {
            return false;
        }
    } else {
        if (_placedPlayerTwoShipCount[shipSizeToIndex] < _placedPlayerTwoShipCountLimit[shipSizeToIndex]) {
            return false;
        }
    }

    return true;
}

void Game::OnClickSquare() {

    if (_mouseX < _tableSize * _gridSize) {
        if (!GetGameMode()) {
            SaveShip();
            cout << "OnClick our: " << endl;
        }
    } else {
        if (GetGameMode()) {
            SaveShot();
            cout << "OnClick enemy: " << endl;
        }
    }
    SetRotateAdjust();
}

void Game::OnScrollSquare(const float &preciseY) {
    if (_mouseX < _tableSize * _gridSize) {
        scrollShipSize(preciseY);
        cout << "OnScroll: " << endl;
        SetRotateAdjust();
    }
}

void Game::scrollShipSize(const float &preciseY){
    int newSize = preciseY > 0 ? _shipSize + 1 : _shipSize - 1;
    if (newSize <= 4 && newSize > 0) {
        _shipSize = newSize;
    }
}

void::Game::RotateShip(){
    setIsVertical(!getIsVertical());
    SetRotateAdjust();
}

void Game::SaveShip() {
    // confirm validation
    if (_indexX >= _tableSize
        || (_indexY >= _tableSize && _indexY < _tableSize + 2)
        || _indexY >= _tableSize * 2 + 2) {
        return;
    }

    Boat newBoat = Boat(_highlightedBoat, getShipSize(), getIsVertical());

    const bool isPlayerOne = _indexY < _tableSize;
    if (IsBoatCollideOtherBoats(newBoat, isPlayerOne ? _playerOneGridBoats : _playerTwoGridBoats)
        || IsPaddingCollideBoats(newBoat, isPlayerOne ? _playerOneGridBoats : _playerTwoGridBoats)
        || IsShipLimitReached(isPlayerOne)) {
        return;
    }

    const int shipSizeToIndex = getShipSize() - 1;
    if (isPlayerOne) {
        ++_placedPlayerOneShipCount[shipSizeToIndex];
        _playerOneGridBoats.emplace_back(newBoat);
    } else {
        ++_placedPlayerTwoShipCount[shipSizeToIndex];
        _playerTwoGridBoats.emplace_back(newBoat);
    }
    ChangeGameMode();
}

bool Game::IsAllShipPartDamaged(const Boat *damagedBoat) {
    return std::all_of(damagedBoat->body.begin(), damagedBoat->body.end(),
                       [](const auto &part) { return part._color == 0xffff00; });
}

SDL_Rect Game::GetShotPadding(bool isPlayerOne, Boat *damagedBoat) {
    SDL_Rect shipPadding = GetShipsPadding(*damagedBoat);
    shipPadding = MapFromShipGridToShotGrid(isPlayerOne, shipPadding);
    shipPadding = CutShotOutsideGridPlayer(isPlayerOne, shipPadding);
    //cout << "shipPadding.x y" << shipPadding.x << " " << shipPadding.y << endl; //deal damage

    return shipPadding;
}

bool Game::MakeShot(const SDL_Rect &shotRect, const SDL_Rect &damageRect, vector<Boat> &playerBoats,
                    vector<GridShot> &playerShots, bool isPlayerOne) {  //NOTE: player two shots
    if (auto damagedBoat = ShotCollideOtherBoats(damageRect, playerBoats); damagedBoat != nullptr) {
        if (IsAllShipPartDamaged(damagedBoat)) {
            playerShots.emplace_back(GetShotPadding(isPlayerOne, damagedBoat));
            playerShots.emplace_back(shotRect, 0xffff00);
            cout << "kill" << endl; //deal damage

            return true;
        } else {
            playerShots.emplace_back(shotRect, 0xffff00);
            cout << "hit" << endl; //deal damage

            return true;
        }
    } else {
        playerShots.emplace_back(shotRect);
        cout << "miss" << endl;
    }

    return false;
}

SDL_Rect Game::MapFromShotGridToShipGrid(bool isPlayerOne, const int x, const int y) {
    if (isPlayerOne) {
        return SDL_Rect{(x - _tableSize - 2) * _gridSize,
                        (y + _tableSize + 2) * _gridSize,
                        _gridSize,
                        _gridSize};
    } else {
        return SDL_Rect{(x - _tableSize - 2) * _gridSize,
                        (y - _tableSize - 2) * _gridSize,
                        _gridSize,
                        _gridSize};
    }
}

SDL_Rect Game::MapFromShipGridToShotGrid(bool isPlayerOne, SDL_Rect shipPadding) const {
    if (isPlayerOne) {
        return SDL_Rect{shipPadding.x + (_tableSize + 2) * _gridSize,
                        shipPadding.y - (_tableSize + 2) * _gridSize,
                        shipPadding.w,
                        shipPadding.h};
    } else {
        return SDL_Rect{shipPadding.x + (_tableSize + 2) * _gridSize,
                        shipPadding.y + (_tableSize + 2) * _gridSize,
                        shipPadding.w,
                        shipPadding.h};
    }
}

void Game::SaveShot() {
    const int x = _indexX;
    const int y = _indexY;
    constexpr int gridPadding = 2;
    if (x >= _tableSize * 2 + gridPadding //right side after two grids
        || (y >= _tableSize && y < _tableSize + gridPadding) //vertical grid padding
        || (x >= _tableSize && x < _tableSize + gridPadding) //horizontal grid padding
        || y >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    cout << "SaveShot x: " << x << " y:" << y << endl;
    const SDL_Rect shotRect = SDL_Rect{x * _gridSize, y * _gridSize, _gridSize, _gridSize};

    cout << "shotRect x: " << shotRect.x << " y:" << shotRect.y << endl;

    if (_isPlayerOneTurn) {
        if (y < _tableSize) {
            if (CheckEnemyShotsCollision(shotRect, _playerOneGridShots)) {
                constexpr bool isPlayerOne = true;
                const SDL_Rect &shipRect = MapFromShotGridToShipGrid(isPlayerOne, _indexX, _indexY);
                if (!MakeShot(shotRect, shipRect, _playerTwoGridBoats, _playerOneGridShots, isPlayerOne)) {
                    _isPlayerOneTurn = false;
                }
            }
        }
    } else {
        if (y >= _tableSize + gridPadding && y < _tableSize * 2 + gridPadding) {
            if (CheckEnemyShotsCollision(shotRect, _playerTwoGridShots)) {
                constexpr bool isPlayerOne = false;
                const SDL_Rect &hitShipRect = MapFromShotGridToShipGrid(isPlayerOne, _indexX, _indexY);
                if (!MakeShot(shotRect, hitShipRect, _playerOneGridBoats, _playerTwoGridShots, isPlayerOne)) {
                    _isPlayerOneTurn = true;
                }
            }
        }
    }
}

SDL_Rect Game::GetShipsPadding(const Boat &boat) {
    const auto head = (boat.body.begin())->getRect();
    const auto paddingHead = SDL_Rect{head.x - head.w - 1, head.y - head.h - 1};

    const auto tail = (--boat.body.end())->getRect();
    const int shipSize = (tail.w + 1) * 2 - 1;
    const auto paddingTail = SDL_Rect{tail.x + shipSize, tail.y + shipSize};
    return SDL_Rect{
            paddingHead.x, paddingHead.y, paddingTail.x - paddingHead.x, paddingTail.y - paddingHead.y};
}

void Game::drawPlayersShips(SDL_Renderer *renderer) {
    //Draw player one ships
    for (const auto &boat: _playerOneGridBoats) {
        //draw ship padding
        if (_indexX < _tableSize && _indexY < _tableSize) {
            //count ship padding
            auto paddingRect = GetShipsPadding(boat);
            paddingRect = CutOutsideGridPlayerOne(paddingRect);

            SDL_SetRenderDrawColor(renderer, 70, 0, 0, 255);
            SDL_RenderFillRect(renderer, &paddingRect);
        }

        //draw ship
        for (const auto &boatPart: boat.body) {
            const auto rect = boatPart.getRect();
            if (boatPart._color == 0x0000ff) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //TODO: create enum pallet for colors
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    //Draw player two ships
    for (const auto &boat: _playerTwoGridBoats) {
        //draw ship padding
        if (_indexX < _tableSize
            && _indexY >= _tableSize + 2
            && _indexY < _tableSize * 2 + 2) {

            auto paddingRect = GetShipsPadding(boat);
            paddingRect = CutOutsideGridPlayerTwo(paddingRect);

            SDL_SetRenderDrawColor(renderer, 70, 0, 0, 255);
            SDL_RenderFillRect(renderer, &paddingRect);
        }

        //draw ship
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //TODO: create enum pallet for colors
        for (const auto &boatPart: boat.body) {
            const auto rect = boatPart.getRect();
            if (boatPart._color == 0x0000ff) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //TODO: create enum pallet for colors
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

SDL_Rect &Game::CutOutsideGridPlayerOne(SDL_Rect &paddingRect) const {
    if (paddingRect.x + paddingRect.w > _tableSize * _gridSize) {
        paddingRect.w -= _gridSize; //cut width outside the grid
    }
    if (paddingRect.y + paddingRect.h > _tableSize * _gridSize) {
        paddingRect.h -= _gridSize; //cut height outside the grid
    }
    return paddingRect;
}

SDL_Rect &Game::CutOutsideGridPlayerTwo(SDL_Rect &paddingRect) const {
    //cut left padding if outside the grid by x
    //if (paddingRect.x < 0) {
    //    paddingRect.x += _gridSize; //cut width of outside grid
    //}

    //cut left padding if outside the grid by y
    if (paddingRect.y < (_tableSize + 2) * _gridSize) {
        paddingRect.y += _gridSize + 1;
        paddingRect.h -= _gridSize;
    }

    //cut right padding if outside the grid by x
    if (paddingRect.x + paddingRect.w > _tableSize * _gridSize) {
        paddingRect.w -= _gridSize; //cut width of outside grid
    }

    //cut bottom padding if outside the grid by y
    if (paddingRect.y + paddingRect.h > (_tableSize * 2 + 2) * _gridSize) {
        paddingRect.h -= _gridSize; //cut height if outside the grid
    }

    return paddingRect;
}

SDL_Rect &Game::CutShotOutsideGridPlayer(bool isPlayerOne, SDL_Rect &paddingRect) const {
    if (isPlayerOne) {
        if (paddingRect.x < (_tableSize + 2) * _gridSize) { //cut left outside the grid
            paddingRect.x += _gridSize;
            paddingRect.w -= _gridSize;
        }

        if (paddingRect.y + paddingRect.h > _tableSize * _gridSize) { //cut bottom outside the grid
            paddingRect.h -= _gridSize;
        }

        if (paddingRect.x + paddingRect.w > (_tableSize * 2 + 2) * _gridSize) { //cut right outside the grid
            paddingRect.w -= _gridSize;
        }
    } else {
        if (paddingRect.y < (_tableSize + 2) * _gridSize) { //cut top padding outside the grid
            paddingRect.y += _gridSize;
            paddingRect.h -= _gridSize;
        }

        if (paddingRect.x < (_tableSize + 2) * _gridSize) { //cut left padding outside the grid
            paddingRect.x += _gridSize;
            paddingRect.w -= _gridSize;
        }

        if (paddingRect.y + paddingRect.h > (_tableSize * 2 + 2) * _gridSize) { //cut bottom padding outside the grid
//        paddingRect.y += _gridSize;
            paddingRect.h -= _gridSize;
        }

        if (paddingRect.x + paddingRect.w > (_tableSize * 2 + 2) * _gridSize) { //cut right padding outside the grid
//        paddingRect.x += _gridSize;
            paddingRect.w -= _gridSize;
        }
    }

    return paddingRect;
}

void Game::drawPlayerShots(SDL_Renderer *renderer) {
    for (const auto &enemyFieldShot: _playerOneGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();

        if (enemyFieldShot._color == 0x700000) { //Draw padding
            SDL_SetRenderDrawColor(renderer, 0x70, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    for (const auto &enemyFieldShot: _playerOneGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();

        if (enemyFieldShot._color == 0xffff00) { //Draw ships
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    for (const auto &enemyFieldShot: _playerTwoGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();

        if (enemyFieldShot._color == 0x700000) { //Draw padding
            SDL_SetRenderDrawColor(renderer, 0x70, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    for (const auto &enemyFieldShot: _playerTwoGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();

        if (enemyFieldShot._color == 0xffff00) { //Draw ships
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Game::drawHighlightedShip(SDL_Renderer *renderer) {
    const int x = _indexX;
    const int y = _indexY;
    constexpr int gridPadding = 2;
    if (x >= _tableSize //right side after one grids
        || (y >= _tableSize && y < _tableSize + gridPadding) //vertical grid padding
        || y >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    _highlightedBoat.x = x * _gridSize + 1;//TODO: extract setting from drawing logic
    _highlightedBoat.y = y * _gridSize + 1;

    Boat newBoat = Boat(_highlightedBoat, getShipSize(), getIsVertical());
    const bool isPlayerOne = y < _tableSize;
    vector<Boat> &otherBoats = isPlayerOne ? _playerOneGridBoats : _playerTwoGridBoats;
    if (IsBoatCollideOtherBoats(newBoat, otherBoats)
        || IsPaddingCollideBoats(newBoat, otherBoats)
        || IsShipLimitReached(y < _tableSize)) {
        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }

    const auto hRect = _highlightedBoat;
    const int size = getShipSize();
    const bool isVertical = getIsVertical();

    int xRect;
    int yRect;
    for (int i = 0; i < size; ++i) {
        if (isVertical) {
            xRect = hRect.x;
            yRect = hRect.y + (hRect.h + 1) * i;
        } else {
            xRect = hRect.x + (hRect.w + 1) * i;
            yRect = hRect.y;
        }

        auto rect = SDL_Rect{xRect, yRect, hRect.w, hRect.h};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::render([[maybe_unused]] SDL_Renderer *renderer) {}

void Game::setIsVertical(bool isVertical) { _isVertical = isVertical; }

void Game::setShipSize(int shipSize) {
    _shipSize = shipSize;
    SetRotateAdjust();
}

bool Game::getIsVertical() const { return _isVertical; }

int Game::getShipSize() const { return _shipSize; }
