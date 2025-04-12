#include "Application.h"
#include "Renderer.h"
#include "GameTable.h"
#include <iostream>

using namespace std;

Application::Application() = default;

bool Application::init() {
    this->renderer = new Renderer();

    // Will initialize window and renderer SDL on init() action
    if (renderer->init()) {
        game.init();

        return true;
    }

    return false;
}

Application::~Application() = default;

void Application::gameLoop() {
    const auto *playerShipFild = new GameTable();
    const auto *playerShotFild = new GameTable({12, 0, 40, 40});

    const auto *enemyShipFild = new GameTable({0, 12, 40, 40});
    const auto *enemyShotFild = new GameTable({12, 12, 40, 40});

    while (handleInput()) {
        // Start counting time
        frameStart = SDL_GetTicks();
        // Clear "buffer" before every screen generation
        renderer->clear();

        game.drawShipsList(renderer->renderer);
        game.drawPlayerShotList(renderer->renderer);

        // Draw highlighted square
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

        calculateFPS();
    }
}

bool Application::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                game.setIsVertical(!game.getIsVertical());
                game.setRotateAdjust();
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_1) {
                game.setShipSize(1);
                game.setRotateAdjust();
            } else if (e.key.keysym.scancode == SDL_SCANCODE_2) {
                game.setShipSize(2);
                game.setRotateAdjust();
            } else if (e.key.keysym.scancode == SDL_SCANCODE_3) {
                game.setShipSize(3);
                game.setRotateAdjust();
            } else if (e.key.keysym.scancode == SDL_SCANCODE_4) {
                game.setShipSize(4);
                game.setRotateAdjust();
            }

            cout << "Event KEY DOWN: " << e.key.keysym.scancode << endl;
        }

        if (e.type == SDL_MOUSEMOTION) {
            game.setMouseCoordinates(e.button.x, e.button.y);
            game.setSquareLocation(e.button.x, e.button.y);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            cout << "Event MOUSE BUTTON DOWN : " << "x: " << e.button.x / 40 << " y: " << e.button.y / 40 << endl;
            game.OnClickSquare();
        }
    }

    return true;
}

void Application::calculateFPS() {
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= 1000) {
        cout << "FPS: " << frameCount << endl;
        frameCount = 0;
        lastTime = currentTime;
    }
}