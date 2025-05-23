#include <algorithm>
#include <Boat.h>

Boat::Boat(SDL_Rect rect) {
    body.emplace_back(rect);
}

Boat::Boat(const std::list<BoatPart> &body) : body{body} {
}

BoatPart::BoatPart(const SDL_Rect rect) : _rect{rect} {
}

SDL_Rect BoatPart::GetRect() const { return _rect; }

// Setter for rect
void BoatPart::SetRect(const SDL_Rect &newRect) {
    _rect = newRect;
}

int BoatPart::GetColor() const {
    return _color;
}

void BoatPart::SetColor(const int color) {
    _color = color;
}

bool BoatPart::IsDead() const {
    return _isDead;
}

void BoatPart::SetIsDead(const bool isBoat) {
    _isDead = isBoat;
}

Boat::Boat(const SDL_Rect rect, const int size, const bool isVertical) : _size{size} {
    for (int i = 0; i < size; i++) {
        if (isVertical) {
            body.emplace_back(SDL_Rect{rect.x, rect.y + (rect.h + 1) * i, rect.w, rect.h});
        } else {
            body.emplace_back(SDL_Rect{rect.x + (rect.w + 1) * i, rect.y, rect.w, rect.h});
        }
    }
}

int Boat::GetSize() const { return _size; }

void Boat::SetSize(const int size) { _size = size; }

bool Boat::IsDead() const {
    if (!_isDead) {
        return std::all_of(body.begin(), body.end(), [](const BoatPart &boat) { return boat.IsDead(); });
    }
    return _isDead;
}

void Boat::SetIsDead(const bool isBoat) {
    _isDead = isBoat;
}
