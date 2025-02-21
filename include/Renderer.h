#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool init();
    void clear();
    void present();
    void kill();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif // RENDERER_H