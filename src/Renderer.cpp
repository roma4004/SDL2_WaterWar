#include "Renderer.h"

Renderer::Renderer() : window(nullptr), renderer(nullptr) {}

Renderer::~Renderer() {
    KillSdl();
}

bool Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow(
            "SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }

    return true;
}

void Renderer::Clear() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::Present() const {
    SDL_RenderPresent(renderer);
}

void Renderer::SetDrawColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) const {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::DrawLine(const int x1, const int y1, const int x2, const int y2) const {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Renderer::KillSdl() { // move code to render destructor
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