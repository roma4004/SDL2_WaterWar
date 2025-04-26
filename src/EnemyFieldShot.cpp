#include <GridShot.h>

GridShot::GridShot(SDL_Rect rect) : _rect{rect} {}

GridShot::GridShot(SDL_Rect rect, int color) : _rect{rect}, _color{color} {}

SDL_Rect GridShot::getRect() const { return _rect; }

void GridShot::setRect(const SDL_Rect &newRect) {
    _rect = newRect;
}