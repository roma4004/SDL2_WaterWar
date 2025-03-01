#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

class Renderer {

    SDL_Window* window;

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawLine(int x1, int y1, int x2, int y2);

public:
    SDL_Renderer* renderer;

    Renderer();

    ~Renderer();

    bool init();
    void clear();
    void present();
    void killSdl();
};

#endif // RENDERER_H