#include "Application.h"
#include "Renderer.h"
#include "GameTable.h"
#include <iostream>

using namespace std;

Application::Application() = default;

bool Application::Init() {
    this->renderer = new Renderer();

    // Will initialize the window and renderer SDL on init() action
    if (renderer->init()) {
        game.init();

        return true;
    }

    return false;
}

Application::~Application() = default;

void Application::GameLoop() {
    const auto *playerShipFild = new GameTable();
    const auto *playerShotFild = new GameTable({12, 0, 40, 40});

    const auto *enemyShipFild = new GameTable({0, 12, 40, 40});
    const auto *enemyShotFild = new GameTable({12, 12, 40, 40});

    while (HandleInput()) {
        // Start counting time
        frameStart = SDL_GetTicks();
        // Clear "buffer" before every screen generation
        renderer->Clear();

        game.drawPlayersShips(renderer->renderer);
        game.drawPlayerShots(renderer->renderer);

        game.drawHighlightedShip(renderer->renderer);

        playerShipFild->draw(renderer->renderer);
        playerShotFild->draw(renderer->renderer);

        enemyShipFild->draw(renderer->renderer);
        enemyShotFild->draw(renderer->renderer);

        //        game.render(renderer->renderer);

        SDL_RenderPresent(renderer->renderer);

        // Starting time minus time spend on handling events, updating and rendering
        frameTime = SDL_GetTicks() - frameStart;

        // Keep 60 FPS
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        CalculateFPS();
    }
}

bool Application::HandleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        if (!game.GetIsGameOver()) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                    game.RotateShip();
                }

                if (e.key.keysym.scancode == SDL_SCANCODE_1) {
                    game.setShipSize(1);
                } else if (e.key.keysym.scancode == SDL_SCANCODE_2) {
                    game.setShipSize(2);
                } else if (e.key.keysym.scancode == SDL_SCANCODE_3) {
                    game.setShipSize(3);
                } else if (e.key.keysym.scancode == SDL_SCANCODE_4) {
                    game.setShipSize(4);
                }

                cout << "Event KEY DOWN: " << e.key.keysym.scancode << endl;
            }

            if (e.type == SDL_MOUSEMOTION) {
                game.setMouseCoordinates(e.button.x, e.button.y);
                game.setSquareLocation(e.button.x, e.button.y);
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                cout << "Event MOUSE BUTTON DOWN : "
                        << "x: " << e.button.x / 40
                        << " y: " << e.button.y / 40 << endl;

                if (e.button.button == SDL_BUTTON_LEFT) {
                    game.OnClickSquare();
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    game.RotateShip();
                }
            }
            if (e.type == SDL_MOUSEWHEEL) {
                cout << "Event MOUSE WHEEL SCROLL : "
                        << "x: " << e.wheel.preciseX
                        << " y: " << e.wheel.preciseY << endl;
                game.OnScrollSquare(e.wheel.preciseY);
            }
        }
    }
    return true;
}

void Application::CalculateFPS() {
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= 1000) {
        cout << "FPS: " << frameCount << endl;
        frameCount = 0;
        lastTime = currentTime;
    }
}
