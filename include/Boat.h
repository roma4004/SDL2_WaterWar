#ifndef BOAT_H
#define BOAT_H

#include <SDL.h>
#include <list>

class BoatPart {
    SDL_Rect _rect{};

public:
    explicit BoatPart(SDL_Rect rect);

    [[nodiscard]] SDL_Rect getRect() const;

    void setRect(const SDL_Rect &newRect);
};

class Boat {

public:
    std::list<BoatPart> body;

    explicit Boat(SDL_Rect rect);

    explicit Boat(std::list<BoatPart> &rect);

    explicit Boat(SDL_Rect rect, int size, bool isVertical);
};


#endif //BOAT_H
