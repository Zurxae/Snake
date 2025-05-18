#define SDL_MAIN_USE_CALLBACKS 1
#include "SDLWindow.hpp"
#include "SnakeGame.hpp"
#include <SDL3/SDL_main.h>

const std::string title = "Snake";
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

struct AppStates {
    SnakeGame* snakeGame;

    ~AppStates() {
        delete snakeGame;
    }
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    int rows = 10, cols = 10;
    if (argc < 3) {
        SDL_Log("Using default values for numRows and numColumns");
    } else {
        rows = std::stoi(argv[1]);
        cols = std::stoi(argv[2]);
    }

    AppStates* state = new AppStates();
    state->snakeGame = new SnakeGame(WINDOW_WIDTH, WINDOW_HEIGHT, rows, cols);

    if (!state->snakeGame->isInitialized()) {
        SDL_Log("Failed to initialize Snake Game");
        return SDL_APP_FAILURE;
    }

    *appstate = state;

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
    AppStates* state = static_cast<AppStates*>(appstate);

    if (state->snakeGame->restartGame) {
        state->snakeGame->setGame();
    }

    state->snakeGame->updateGameState();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppStates* state = static_cast<AppStates*>(appstate);

    delete state;

    SDL_Log("Deleted AllStates");    
}
