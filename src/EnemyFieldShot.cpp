#include <GridShot.h>

int GridShot::GetColor() const {
    return _color;
}

void GridShot::SetColor(const int &color) {
    _color = color;
}

GridShot::GridShot(SDL_Rect rect) : _rect{rect} {}

GridShot::GridShot(SDL_Rect rect, int color, const bool &isBoat) : _rect{rect}, _color{color}, _isBoat {isBoat} {}

SDL_Rect GridShot::getRect() const { return _rect; }

void GridShot::setRect(const SDL_Rect &newRect) {
    _rect = newRect;
}

bool GridShot::IsBoat() const {
    return _isBoat;
}

void GridShot::SetBoat(const bool &boat) {
    _isBoat = boat;
}

