#include <EnemyGridShot.h>

EnemyGridShot::EnemyGridShot(SDL_Rect rect) : _rect{rect} {}

SDL_Rect EnemyGridShot::getRect() const { return _rect; }

void EnemyGridShot::setRect(const SDL_Rect &newRect) {
    _rect = newRect;
}