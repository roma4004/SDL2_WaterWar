#include "Application.h"
#include <iostream>

using namespace std;

// Define static members
SDL_Window* Application::window = nullptr;
SDL_Renderer* Application::renderer = nullptr;

// Debug
bool Application::fps_debugging = false;
bool Application::snake_position_debugging = false;


Application::Application() : frameCount(0), lastTime(0) {}

Application::~Application() {
    kill();
}

bool Application::init() {
    // Initialize SDL and create window and renderer
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }
    game.init();
    
    return true;
}

void Application::run() {
    Uint32 frameStart;
    int frameTime;
    int screen_width = 720;
    int screen_height = 480;
    int grid_size = 40;
    int fild_size = 11;
    int size = grid_size * (fild_size -1);
    

    while (loop()) {
        // Start counting time
        frameStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        for (int y = 0; y < fild_size; y++) {
            for (int x = 0; x < fild_size; x++){
                SDL_RenderDrawLine(renderer, x * grid_size, 0, x * grid_size, size);
            }
            SDL_RenderDrawLine(renderer, 0, y * grid_size,  size, y * grid_size);
        }

        game.update();

        game.render(renderer);
        // SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        // Starting time minus time spend on handling events, updating and rendering
        frameTime = SDL_GetTicks() - frameStart;

        // Keep 60 FPS
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        calculateFPS();
    }
}

bool Application::loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        if (e.type == SDL_KEYDOWN) {
            cout << "Event type: " << e.key.keysym.scancode << endl;    
        }
        if (e.type == SDL_MOUSEMOTION) {
            game.setMouseCoordinates(e.motion.x, e.motion.y);
            game.setSquareLocation(e.motion.x, e.motion.y);
        }
    }

    return true;
}

void Application::kill() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
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