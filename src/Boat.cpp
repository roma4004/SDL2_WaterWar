#include <Boat.h>

Boat::Boat(SDL_Rect rect) {
    body.emplace_back(rect);
}

Boat::Boat(std::list<BoatPart> &lst) : body{lst} {}

BoatPart::BoatPart(SDL_Rect rect) : _rect{rect} {}

SDL_Rect BoatPart::getRect() const { return _rect; }

// Setter for rect
void BoatPart::setRect(const SDL_Rect &newRect) {
    _rect = newRect;
}

Boat::Boat(SDL_Rect rect, int size, bool isVertical) {
    for (int i = 0; i < size; i++) {
        if (isVertical) {
            body.emplace_back(SDL_Rect{rect.x, rect.y + (rect.h + 1) * i, rect.w, rect.h});
        } else {
            body.emplace_back(SDL_Rect{rect.x + (rect.w + 1) * i, rect.y, rect.w, rect.h});
        }
    }
}
