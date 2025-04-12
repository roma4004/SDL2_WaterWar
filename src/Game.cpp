#include "Game.h"
#include "EnemyGridShot.h"

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
    this->_indexX = this->_mouseX / this->_gridSize;
    this->_indexY = this->_mouseY / this->_gridSize;

    constexpr int gridPadding = 2; //TODO: should be class member
    if (this->_indexX >= _tableSize //right side after one grids
        ||
        (this->_indexY >= _tableSize && this->_indexY < _tableSize + gridPadding) //vertical padding between grids
        || this->_indexY >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    if (_isVertical) {
        const int shipHead = this->_indexY;
        const int shipTail = this->_indexY + _shipSize;
        if ((shipTail > this->_tableSize &&
             shipTail <= this->_tableSize + gridPadding) //vertical padding between grids
            || shipTail > this->_tableSize * 2 + gridPadding //bottom side after two grids
            || (shipHead == this->_tableSize - 1 &&
                shipTail > this->_tableSize + gridPadding) //case when tail in different grid
                ) {
            if (shipHead > this->_tableSize + gridPadding) {
                this->_indexY = this->_tableSize * 2 + gridPadding - _shipSize;
            } else {
                this->_indexY = this->_tableSize - _shipSize;
            }
        }
    } else {
        const int shipTail = this->_indexX + _shipSize;
        if (shipTail > this->_tableSize) {
            this->_indexX = this->_tableSize - _shipSize;
        }
    }

    cout << "adjusted to: " << "x: " << this->_indexX << " y: " << this->_indexY << endl;
}

void Game::setMouseCoordinates(int x, int y) {
    this->_mouseX = x;
    this->_mouseY = y;
    cout << "setMouseCoordinates x: " << this->_mouseX << " y: " << this->_mouseY << endl;
}

void Game::setSquareLocation(int x, int y) {
    this->_indexX = x / this->_gridSize;
    this->_indexY = y / this->_gridSize;

    constexpr int gridPadding = 2;
    if (this->_indexX >= _tableSize //right side after one grids
        ||
        (this->_indexY >= _tableSize && this->_indexY < _tableSize + gridPadding) //vertical padding between grids
        || this->_indexY >= _tableSize * 2 + gridPadding) {  //bottom side after grids
        return;
    }

    if (_isVertical) {
        const int shipHead = this->_indexY;
        const int shipTail = this->_indexY + _shipSize;
        if ((shipTail > this->_tableSize &&
             shipTail <= this->_tableSize + gridPadding) //vertical padding between grids
            || shipTail > this->_tableSize * 2 + gridPadding//bottom side after grids
            || (shipHead == this->_tableSize - 1 &&
                shipTail > this->_tableSize + gridPadding)) { //case when tail in different grid

            if (shipHead > this->_tableSize + gridPadding) {
                this->_indexY = this->_tableSize * 2 + gridPadding - _shipSize;
            } else {
                this->_indexY = this->_tableSize - _shipSize;
            }

            return;
        }
    } else {
        const int shipTail = this->_indexX + _shipSize;
        if (shipTail > this->_tableSize) {
            this->_indexX = this->_tableSize - _shipSize;
            return;
        }
    }


    cout << "Square location: " << "x: " << this->_indexX << " y: " << this->_indexY << endl;
}

bool Game::IsCollide(const SDL_Rect &r1, const SDL_Rect &r2) {
    // Check if one rectangle is to the right of the other
    if (r1.x > r2.x + r2.w || r2.x > r1.x + r1.w) {
        return false;
    }

    // Check if one rectangle is above the other
    if (r1.y > r2.y + r2.h || r2.y > r1.y + r1.h) {
        return false;
    }

    // If neither of the above conditions are met, the rectangles overlap
    return true;
}

bool Game::CheckEnemySquareCollision(const SDL_Rect &newShot, const std::vector<EnemyGridShot> &enemyFieldShots) {
    return std::none_of(enemyFieldShots.begin(), enemyFieldShots.end(), [&newShot](const EnemyGridShot &shot) {
        const auto& oldShot = shot.getRect();
        return newShot.x == oldShot.x && newShot.y == oldShot.y;
    });
}

bool Game::CheckPaddingCollision(const Boat &boat1, const std::vector<Boat> &otherBoats) const {
    const auto paddingHead = SDL_Rect{highlightedSquare.x - highlightedSquare.w - 1,
                                      highlightedSquare.y - highlightedSquare.h - 1};
    auto paddingTail = (--(boat1.body.end()))->getRect();
    paddingTail = {paddingTail.x + paddingTail.w + 1, paddingTail.y + paddingTail.h + 1};
    const auto paddingRect = SDL_Rect{paddingHead.x,
                                      paddingHead.y,
                                      paddingTail.x - paddingHead.x,
                                      paddingTail.y - paddingHead.y};
    for (const auto &boat: otherBoats) {
        for (const auto &boatPart: boat.body) {
            const auto rect2 = boatPart.getRect();
            if (IsCollide(paddingRect, rect2)) {
                return false;
            }
        }
    }

    return true;
}

bool Game::CheckDirectCollision(const Boat &boat1, const std::vector<Boat> &otherBoats) {
    for (const auto &highlightedPart: boat1.body) {
        const auto rect = highlightedPart.getRect();
        for (const auto &boat: otherBoats) {
            for (const auto &boatPart: boat.body) {
                const auto rect2 = boatPart.getRect();
                if (rect.x == rect2.x && rect.y == rect2.y) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Game::CheckShipLimits() {
    const int shipSizeToIndex = getShipSize() - 1;
    if (_placedShipCount[shipSizeToIndex] < _placedShipCountLimit[shipSizeToIndex]) {
        return true;
    }

    return false;
}

void Game::OnClickSquare() {
    if (this->_mouseX <= _tableSize * _gridSize) {
        SaveSelectedSquare();
        cout << "OnClick our: " << endl;
    } else {
        SaveEnemySquare();
        cout << "OnClick enemy: " << endl;
    }
}

void Game::SaveSelectedSquare() {
    // confirm validation
    if (this->_indexX >= this->_tableSize
        || this->_indexY >= this->_tableSize) {
        return;
    }

    Boat newBoat = Boat(this->highlightedSquare, getShipSize(), getIsVertical());

    if (CheckDirectCollision(newBoat, this->_ourGridBoats)
        && CheckPaddingCollision(newBoat, this->_ourGridBoats)
        && CheckShipLimits()) {
        const int shipSizeToIndex = getShipSize() - 1;
        ++_placedShipCount[shipSizeToIndex];
        this->_ourGridBoats.emplace_back(newBoat);
    }
}

void Game::SaveEnemySquare() {
    int x = this->_indexX;
    int y = this->_indexY;
    constexpr int gridPadding = 2;
    if (x >= _tableSize * 2 + gridPadding //right side after two grids
        || (y >= _tableSize && y < _tableSize + gridPadding) //vertical padding between grids
        || (x >= _tableSize && x < _tableSize + gridPadding) //horizontal padding between grids
        || y >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    cout << "SaveEnemySquare x: " << x << endl;
    cout << "SaveEnemySquare y: " << y << endl;
    const SDL_Rect enemySquare = SDL_Rect{x * this->_gridSize, y * this->_gridSize, this->_gridSize, this->_gridSize};
    if (CheckEnemySquareCollision(enemySquare, this->_enemyGridShots)) {
        this->_enemyGridShots.emplace_back(enemySquare);
    }
}

void Game::drawShipsList(SDL_Renderer *renderer) {
    for (const auto &boat: this->_ourGridBoats) {

        if (!(this->_indexX > this->_tableSize - 1
              || this->_indexY > this->_tableSize - 1)) {

            //count ship padding
            auto boatHeadRect = (*boat.body.begin()).getRect();
            const auto paddingHead = SDL_Rect{boatHeadRect.x - (boatHeadRect.x <= 1 ? 0 : boatHeadRect.w) - 1,
                                              boatHeadRect.y - (boatHeadRect.y <= 1 ? 0 : boatHeadRect.h) - 1};

            auto paddingTail = (--boat.body.end())->getRect();
            int shipSize = (paddingTail.w + 1) * 2 - 1;
            paddingTail = {paddingTail.x + shipSize, paddingTail.y + shipSize};
            auto paddingRect = SDL_Rect{paddingHead.x,
                                        paddingHead.y,
                                        paddingTail.x - paddingHead.x,
                                        paddingTail.y - paddingHead.y};

            //draw ship padding
            SDL_SetRenderDrawColor(renderer, 70, 0, 0, 255);
            for (const auto &boatPart: boat.body) {
                const auto rect2 = boatPart.getRect();
                if (IsCollide(paddingRect, rect2)) {
                    if (paddingRect.x + paddingRect.w > this->_tableSize * _gridSize) {
                        paddingRect.w -= _gridSize;
                    }
                    if (paddingRect.y + paddingRect.h > this->_tableSize * _gridSize) {
                        paddingRect.h -= _gridSize;
                    }
                    SDL_RenderFillRect(renderer, &paddingRect);
                }
            }
        }

        //draw ship
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (const auto &boatPart: boat.body) {
            const auto rect = boatPart.getRect();
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Game::drawPlayerShotList(SDL_Renderer *renderer) {
    for (const auto &enemyFieldShot: this->_enemyGridShots) {
        const SDL_Rect rect = enemyFieldShot.getRect();
        SDL_SetRenderDrawColor(renderer, 0xFF, 45, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::drawHighlightedShip(SDL_Renderer *renderer) {
    int x = this->_indexX;
    int y = this->_indexY;
    constexpr int gridPadding = 2;
    if (x >= _tableSize //right side after one grids
        || (y >= _tableSize && y < _tableSize + gridPadding) //vertical padding between two grids
        || y >= _tableSize * 2 + gridPadding) {  //bottom side after two grids
        return;
    }

    this->highlightedSquare.x = x * this->_gridSize + 1;
    this->highlightedSquare.y = y * this->_gridSize + 1;

    Boat highlightedBoat = Boat(this->highlightedSquare, getShipSize(), getIsVertical());
    if (CheckDirectCollision(highlightedBoat, this->_ourGridBoats)
        && CheckPaddingCollision(highlightedBoat, this->_ourGridBoats)
        && CheckShipLimits()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
    }

    auto hRect = this->highlightedSquare;
    const int size = getShipSize();
    const bool isVertical = getIsVertical();
    SDL_Rect rect;
    for (int i = 0; i < size; ++i) {
        if (isVertical) {
            rect = SDL_Rect{hRect.x, hRect.y + (hRect.h + 1) * i, hRect.w, hRect.h};
        } else {
            rect = SDL_Rect{hRect.x + (hRect.w + 1) * i, hRect.y, hRect.w, hRect.h};
        }

        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::render(SDL_Renderer *renderer) {}

void Game::setIsVertical(bool isVertical) { _isVertical = isVertical; }

void Game::setShipSize(int shipSize) { _shipSize = shipSize; }

bool Game::getIsVertical() const { return _isVertical; }

int Game::getShipSize() const { return _shipSize; }
