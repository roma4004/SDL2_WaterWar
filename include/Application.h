#ifndef APPLICATION_H
#define APPLICATION_H

#include "Renderer.h"
#include "Game.h"
#include "GameTable.h"

class Application {
public:
    Application();

    ~Application();

    Renderer *renderer{};

    bool Init();

    void GameLoop();

private:
    bool HandleInput();

    void CalculateFPS();

    Game game;

    const int FPS = 60; // Desired FPS
    const int frameDelay = 1000 / FPS; // Frame delay in milliseconds

    Uint32 frameStart{};
    Uint32 frameTime{};
    Uint32 frameCount{};
    Uint32 lastTime{};
};

#endif // APPLICATION_H
