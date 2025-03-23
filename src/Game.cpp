#include "Game.h"
#include <iostream>
#include <SDL.h>

using namespace std;

Game::Game() : gameOver(false), mouseX(0), mouseY(0), indexX(0), indexY(0) {}

Game::~Game() = default;

void Game::init() {}

void Game::update() const {
    if (gameOver) return;
}

void Game::setMouseRotateAdjust() {
    this->indexX = this->mouseX / this->gridSize;
    this->indexY = this->mouseY / this->gridSize;

    if (this->indexX >= this->tableSize
        || this->indexY >= this->tableSize) {
        return;
    }

    if (_isVertical) {
        if (this->indexY + _shipSize > this->tableSize) {
            this->indexY += this->tableSize - (this->indexY + _shipSize);
        }
    } else {
        if (this->indexX + _shipSize > this->tableSize) {
            this->indexX += this->tableSize - (this->indexX + _shipSize);
        }
    }

    cout << "adjusted to: " << "x: " << this->indexX << " y: " << this->indexY << endl;
}

void Game::setMouseCoordinates(int x, int y) {
    this->mouseX = x;
    this->mouseY = y;
    cout << "Event type: " << "x: " << this->mouseX << " y: " << this->mouseY << endl;
}

void Game::setSquareLocation(int x, int y) {
    this->indexX = x / this->gridSize;
    this->indexY = y / this->gridSize;

    if (this->indexX >= this->tableSize
        || this->indexY >= this->tableSize) {
        return;
    }

    if (_isVertical) {
        if (this->indexY + _shipSize > this->tableSize) {
            this->indexY = 10 - _shipSize;
            return;
        }
    } else {
        if (this->indexX + _shipSize > this->tableSize) {
            this->indexX = 10 - _shipSize;
            return;
        }
    }


    cout << "Square location: " << "x: " << this->indexX << " y: " << this->indexY << endl;
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

bool Game::CheckPaddingCollision(const Boat &boat1, const std::vector<Boat> &otherBoats) {
    const auto paddingHead = SDL_Rect{highlightedSquare.x - highlightedSquare.w - 1,
                                      highlightedSquare.y - highlightedSquare.h - 1};
    auto paddingTail = (--boat1.body.end())->getRect();
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
    if (placedShipCount[shipSizeToIndex] < placedShipCountLimit[shipSizeToIndex]) {
        return true;
    }

    return false;
}

void Game::SaveSelectedSquare() {
    // confirm validation

    if (this->indexX >= this->tableSize
        || this->indexY >= this->tableSize) {
        return;
    }

    Boat highlightedBoat = Boat(this->highlightedSquare, getShipSize(), getIsVertical());

    if (CheckDirectCollision(highlightedBoat, this->boats)
        && CheckPaddingCollision(highlightedBoat, this->boats)
        && CheckShipLimits()) {
        const int shipSizeToIndex = getShipSize() - 1;
        ++placedShipCount[shipSizeToIndex];
        this->boats.emplace_back(this->highlightedSquare, getShipSize(), getIsVertical());
    }
}

void Game::drawShipsList(SDL_Renderer *renderer) {
    for (const auto &boat: this->boats) {

        if (!(this->indexX > this->tableSize - 1
              || this->indexY > this->tableSize - 1)) {
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
                    if (paddingRect.x + paddingRect.w > this->tableSize * gridSize) {
                        paddingRect.w -= gridSize;
                    }
                    if (paddingRect.y + paddingRect.h > this->tableSize * gridSize) {
                        paddingRect.h -= gridSize;
                    }
                    SDL_RenderFillRect(renderer, &paddingRect);
                }
            }
        }


        //Direct ship draw
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (const auto &boatPart: boat.body) {
            const auto rect = boatPart.getRect();
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Game::drawHighlightedShip(SDL_Renderer *renderer) {
    if (this->indexX > this->tableSize - 1
        || this->indexY > this->tableSize - 1) {
        return;
    }

    this->highlightedSquare.x = this->indexX * this->gridSize + 1;
    this->highlightedSquare.y = this->indexY * this->gridSize + 1;

    Boat highlightedBoat = Boat(this->highlightedSquare, getShipSize(), getIsVertical());
    if (CheckDirectCollision(highlightedBoat, this->boats)
        && CheckPaddingCollision(highlightedBoat, this->boats)
        && CheckShipLimits()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {

        SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
    }


    auto hRect = this->highlightedSquare;
    const int size = getShipSize();
    const bool isVertical = getIsVertical();
    SDL_Rect rect;
    for (int i = 0; i < size; i++) {
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
