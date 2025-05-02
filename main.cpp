#define SDL_MAIN_USE_CALLBACKS 1
#include "SDLWindow.hpp"
#include "snake.hpp"
#include <SDL3/SDL_main.h>
#include <memory>

const std::string title = "Snake";
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

struct AllStates {
    SDLWindow* window;
    Snake* snakeGame;

    ~AllStates() {
        delete window;
        delete snakeGame;
    }
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    AllStates* allStates = new AllStates();
    allStates->snakeGame = new Snake(10, 10);
    allStates->window = new SDLWindow(title, WINDOW_WIDTH, WINDOW_HEIGHT);
 
    if (!(allStates->window->isInitialized())) {
        return SDL_APP_FAILURE;   
    }

    *appstate = allStates;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    switch(event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            switch(event->key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                case SDL_SCANCODE_Q:
                    return SDL_APP_SUCCESS;
            }
            break;
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    AllStates* allStates = static_cast<AllStates*>(appstate);

    allStates->window->clearScreen(0, 0, 0, SDL_ALPHA_OPAQUE);
    allStates->window->present();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AllStates* allStates = static_cast<AllStates*>(appstate);
    delete allStates;
}
