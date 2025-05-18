#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <SDL3/SDL.h>
#include <string>

class SDLWindow {
public:
    SDLWindow(const std::string& title, int width, int height);
    ~SDLWindow();

    bool isInitialized() const;
    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;

    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void present();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool initialized = false;
};

#endif
