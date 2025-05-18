#include "SDLWindow.hpp"

SDLWindow::SDLWindow(const std::string& title, int width, int height)
    : window(nullptr), renderer(nullptr) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_HIDDEN);

    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    initialized = true;
    SDL_Log("SDLWindow initialized");
}

SDLWindow::~SDLWindow() {
    if (renderer) {
        SDL_Log("Deleting renderer");
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_Log("Deleting window");
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    SDL_Log("Deleted SDLWindow");
}

bool SDLWindow::isInitialized() const {
    return initialized;
}

SDL_Window* SDLWindow::getWindow() const {
    return window;
}

SDL_Renderer* SDLWindow::getRenderer() const {
    return renderer;
}

void SDLWindow::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!initialized) return;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void SDLWindow::present() {
    if (!initialized) return;
    SDL_RenderPresent(renderer);
}