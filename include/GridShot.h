#ifndef ENEMY_FIELD_SHOT_H
#define ENEMY_FIELD_SHOT_H

#include <SDL.h>

class GridShot {
    SDL_Rect _rect{};

public:
    explicit GridShot(SDL_Rect rect);

    [[nodiscard]] SDL_Rect getRect() const;

    void setRect(const SDL_Rect &newRect);
};

#endif //ENEMY_FIELD_SHOT_H
