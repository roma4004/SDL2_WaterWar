#ifndef ENEMY_FIELD_SHOT_H
#define ENEMY_FIELD_SHOT_H

#include <SDL.h>

class GridShot {
    SDL_Rect _rect{};

public:
    int _color{0x700000}; //TODO: move to private and add getter and setter

    explicit GridShot(SDL_Rect rect);

    GridShot(SDL_Rect rect, int color);

    [[nodiscard]] SDL_Rect getRect() const;

    void setRect(const SDL_Rect &newRect);
};

#endif //ENEMY_FIELD_SHOT_H
