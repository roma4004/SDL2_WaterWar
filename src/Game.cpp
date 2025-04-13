#include "Game.h"
#include "GridShot.h"

#include <iostream>
#include <SDL.h>
#include <algorithm>

using namespace std;

Game::Game() : _gameOver(false), _mouseX(0), _mouseY(0), _indexX(0), _indexY(0) {}

Game::~Game() = default;

void Game::init() {}

void Game::update() const {
    if (_gameOver) return;
}

void Game::setRotateAdjust() {
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
    cout << "setMouseCoordinates x: " << _mouseX << " y: " << _mouseY << endl;
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

bool Game::CheckEnemySquareCollision(const SDL_Rect &newShot, const std::vector<GridShot> &enemyFieldShots) {
    return std::none_of(enemyFieldShots.begin(), enemyFieldShots.end(), [&newShot](const GridShot &shot) {
        const auto &oldShot = shot.getRect();
        return newShot.x == oldShot.x && newShot.y == oldShot.y;
    });
}

bool Game::IsBoatsCollidePaddingRect(const Boat &boat, const std::vector<Boat> &otherBoats) {
    const auto paddingRect = CalkShipsPadding(boat);
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

bool Game::IsShipLimitReached() {
    const int shipSizeToIndex = getShipSize() - 1;
    if (_placedShipCount[shipSizeToIndex] < _placedShipCountLimit[shipSizeToIndex]) {
        return false;
    }

    return true;
}

void Game::OnClickSquare() {
    if (_mouseX <= _tableSize * _gridSize) {
        SaveBoat();
        cout << "OnClick our: " << endl;
    } else {
        SaveShot();
        cout << "OnClick enemy: " << endl;
    }
}

void Game::SaveBoat() {
    // confirm validation
    if (_indexX >= _tableSize
        || _indexY >= _tableSize) {
        return;
    }

    Boat newBoat = Boat(_highlightedBoat, getShipSize(), getIsVertical());

    if (IsBoatCollideOtherBoats(newBoat, _playerGridBoats)
        || IsBoatsCollidePaddingRect(newBoat, _playerGridBoats)
        || IsShipLimitReached()) {
        return;
    }

    const int shipSizeToIndex = getShipSize() - 1;
    ++_placedShipCount[shipSizeToIndex];
    _playerGridBoats.emplace_back(newBoat);
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

    cout << "SaveShot x: " << x << endl;
    cout << "SaveShot y: " << y << endl;
    const SDL_Rect enemySquare = SDL_Rect{x * _gridSize, y * _gridSize, _gridSize, _gridSize};
    if (CheckEnemySquareCollision(enemySquare, _playerGridShots)) {
        _playerGridShots.emplace_back(enemySquare);
    }
}

SDL_Rect Game::CalkShipsPadding(const Boat &boat) {
    const auto head = (boat.body.begin())->getRect();
    const auto paddingHead = SDL_Rect{head.x - head.w - 1, head.y - head.h - 1};

    const auto tail = (--boat.body.end())->getRect();
    const int shipSize = (tail.w + 1) * 2 - 1;
    const auto paddingTail = SDL_Rect{tail.x + shipSize, tail.y + shipSize};
    return SDL_Rect{
            paddingHead.x, paddingHead.y, paddingTail.x - paddingHead.x, paddingTail.y - paddingHead.y};
}

void Game::drawPlayerShips(SDL_Renderer *renderer) {
    for (const auto &boat: _playerGridBoats) {

        if (!(_indexX >= _tableSize || _indexY >= _tableSize)) {

            //count ship padding
            auto paddingRect = CalkShipsPadding(boat);

            if (paddingRect.x + paddingRect.w > _tableSize * _gridSize) {
                paddingRect.w -= _gridSize; //cut width if outside grid
            }
            if (paddingRect.y + paddingRect.h > _tableSize * _gridSize) {
                paddingRect.h -= _gridSize; //cut height if outside grid
            }

            //draw ship padding
            SDL_SetRenderDrawColor(renderer, 70, 0, 0, 255);
            SDL_RenderFillRect(renderer, &paddingRect);
        }

        //draw ship
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //TODO: create enum pallet for colors
        for (const auto &boatPart: boat.body) {
            const auto rect = boatPart.getRect();
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Game::drawPlayerShots(SDL_Renderer *renderer) {
    for (const auto &enemyFieldShot: _playerGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();
        SDL_SetRenderDrawColor(renderer, 0xFF, 45, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
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

    _highlightedBoat.x = x * _gridSize + 1;
    _highlightedBoat.y = y * _gridSize + 1;

    Boat newBoat = Boat(_highlightedBoat, getShipSize(), getIsVertical());
    if (IsBoatCollideOtherBoats(newBoat, _playerGridBoats)
        || IsBoatsCollidePaddingRect(newBoat, _playerGridBoats)
        || IsShipLimitReached()) {
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

void Game::render(SDL_Renderer *renderer) {}

void Game::setIsVertical(bool isVertical) { _isVertical = isVertical; }

void Game::setShipSize(int shipSize) { _shipSize = shipSize; }

bool Game::getIsVertical() const { return _isVertical; }

int Game::getShipSize() const { return _shipSize; }
