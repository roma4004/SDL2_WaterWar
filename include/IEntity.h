#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

class IEntity {
public:
    virtual ~IEntity() {}
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual bool checkCollision(const SDL_Rect& other) const = 0;
    virtual SDL_Rect getCollisionBox() const = 0;
};

#endif // ENTITY_H