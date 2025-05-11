#ifndef BOAT_H
#define BOAT_H

#include <SDL.h>
#include <list>

class BoatPart {
    SDL_Rect _rect{};
    int _color{0x0000ff};
    bool _isDead{false};
public:
    explicit BoatPart(SDL_Rect rect);

    [[nodiscard]] SDL_Rect GetRect() const;

    void SetRect(const SDL_Rect &newRect);

    [[nodiscard]] int GetColor() const;
    void SetColor(int color);

    [[nodiscard]] bool IsDead() const;
    void SetIsDead(bool isBoat);
};

class Boat {
    int _size{};
    bool _isDead{false};
public:
    std::list<BoatPart> body;

    explicit Boat(SDL_Rect rect);

    explicit Boat(const std::list<BoatPart> &body);

    explicit Boat(SDL_Rect rect, int size, bool isVertical);

    [[nodiscard]] int GetSize() const;

    void SetSize(int size);

    [[nodiscard]] bool IsDead() const;
    void SetIsDead(bool isBoat);
};


#endif //BOAT_H
