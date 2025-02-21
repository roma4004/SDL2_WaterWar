#ifndef APPLICATION_H
#define APPLICATION_H

#include "Renderer.h"
#include "Game.h"

class Application {
public:
    Application();
    ~Application();
    bool init();
    void run();
    void kill();
    static bool fps_debugging;
    static bool snake_position_debugging;

private:
    bool loop();
    void calculateFPS();

    Game game;

    static SDL_Window* window;
    static SDL_Renderer* renderer;

    const int FPS = 60; // Desired FPS
    const int frameDelay = 1000 / FPS; // Frame delay in milliseconds

    Uint32 frameStart;
    int frameTime;
    int frameCount;
    Uint32 lastTime;
};

#endif // APPLICATION_H