#ifndef ENEMY_FIELD_SHOT_H
#define ENEMY_FIELD_SHOT_H

#include <SDL.h>

class GridShot {
    SDL_Rect _rect{};
    int _color{0x700000};
    bool _isBoat{false};
public:

    [[nodiscard]] int GetColor() const;
    void SetColor (const int &color);
    explicit GridShot(SDL_Rect rect);

    GridShot(SDL_Rect rect, int color, bool isBoat);

    [[nodiscard]] SDL_Rect getRect() const;

    void setRect(const SDL_Rect &newRect);

    [[nodiscard]] bool IsBoat() const;
    void SetBoat(bool boat);

};

#endif //ENEMY_FIELD_SHOT_H
