#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include <string>
#include <random>
#include <queue>
#include <utility>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const std::string title = "Snake";
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

constexpr int ROWS = 30;
constexpr int COLUMNS = 30;
constexpr int ROWSIZE = WINDOW_HEIGHT / ROWS;
constexpr int COLUMNSIZE = WINDOW_WIDTH / COLUMNS;

constexpr int EMPTY = 0;
constexpr int SNAKE = 1;
constexpr int FOOD = 2;

constexpr int UP = 1;
constexpr int RIGHT = 2;
constexpr int DOWN = 3;
constexpr int LEFT = 4;

constexpr Uint64 MOVE_DELAY = 100;

// Random number generator setup (global)
std::random_device rd;
std::mt19937 gen(rd());

// Function to get random integer between min and max (inclusive)
int getRandomInt(int min, int max) {
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

struct GameState {
    int** grid;
    int playerDirection;
    std::queue<std::pair<int, int>> snakeBody;
    Uint64 lastMoveTime;
    bool spawnFood;
    SDL_Window* window;
    SDL_Renderer* renderer;

    ~GameState() {
        if (grid) {
            for (int i = 0; i < ROWS; i++) {
                delete[] grid[i];
            }
            delete[] grid;
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
    }
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

    GameState* gameState = new GameState();
    *appstate = gameState;


    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

     gameState->window = SDL_CreateWindow(title.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);

    if (!(gameState->window)) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gameState->renderer = SDL_CreateRenderer(gameState->window, NULL);

    if (!(gameState->renderer)) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowPosition(gameState->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    gameState->grid = new int*[ROWS];
    for (int i = 0; i < ROWS; i++) {
        gameState->grid[i] = new int[COLUMNS];
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            gameState->grid[i][j] = EMPTY;
        }
    }
    // gameState->snakeBody.push({ROWS / 2, COLUMNS / 2});
    // gameState->grid[gameState->snakeBody.back().first][gameState->snakeBody.back().second] = SNAKE;

    //
    for (int i = 0; i < 10; i++) {
        gameState->snakeBody.push({(ROWS / 2), (COLUMNS / 2)-i});
        gameState->grid[gameState->snakeBody.back().first][gameState->snakeBody.back().second] = SNAKE;
    }
    //

    // int randRow;
    // int randCol;
    // do {
    //     randRow = getRandomInt(0, ROWS - 1);
    //     randCol = getRandomInt(0, COLUMNS - 1);
    // } while (gameState->grid[randRow][randCol] == SNAKE);
    // gameState->grid[randRow][randCol] = FOOD;
    gameState->spawnFood = true;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            std::cout << gameState->grid[i][j];
        }
        std::cout << std::endl;
    }

    gameState->playerDirection = 0;
    gameState->lastMoveTime = 0;

    SDL_ShowWindow(gameState->window);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    GameState* gameState = static_cast<GameState*>(appstate);    

    switch(event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            switch(event->key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                case SDL_SCANCODE_Q:
                    return SDL_APP_SUCCESS;
                case SDL_SCANCODE_UP:
                    gameState->playerDirection = UP;
                    SDL_Log("up");
                    break;
                case SDL_SCANCODE_RIGHT:
                    gameState->playerDirection = RIGHT;
                    SDL_Log("right");
                    break;
                case SDL_SCANCODE_DOWN:
                    SDL_Log("down");
                    gameState->playerDirection = DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    SDL_Log("left");
                    gameState->playerDirection = LEFT;
                    break;
            }
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLUMNS; j++) {
                    std::cout << gameState->grid[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl << std::endl << std::endl;
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

    GameState* gameState = static_cast<GameState*>(appstate);

    Uint64 now = SDL_GetTicks();

    bool shouldMove = false;
    if (now > gameState->lastMoveTime + MOVE_DELAY) {
        shouldMove = true;
        gameState->lastMoveTime = now;
    }

    if (shouldMove) {
        int headRow = gameState->snakeBody.back().first;
        int headColumn = gameState->snakeBody.back().second;
        int tailRow = gameState->snakeBody.front().first;
        int tailColumn = gameState->snakeBody.front().second;

        switch (gameState->playerDirection) {
            case UP:
                if (headRow <= 0) {
                    gameState->playerDirection = 0;
                } else {
                    gameState->grid[tailRow][tailColumn] = EMPTY;
                    gameState->snakeBody.pop();
                    headRow--;
                    if (gameState->grid[headRow][headColumn] == FOOD) {
                        gameState->spawnFood = true;
                    }
                    gameState->grid[headRow][headColumn] = SNAKE;
                    gameState->snakeBody.push({headRow, headColumn});
                }
                break;
            case RIGHT:
                if (headColumn >= COLUMNS - 1) {
                    gameState->playerDirection = 0;
                } else {
                    gameState->grid[tailRow][tailColumn] = EMPTY;
                    gameState->snakeBody.pop();
                    headColumn++;
                    if (gameState->grid[headRow][headColumn] == FOOD) {
                        gameState->spawnFood = true;
                    }
                    gameState->grid[headRow][headColumn] = SNAKE;
                    gameState->snakeBody.push({headRow, headColumn});
                }
                break;
            case DOWN:
                if (headRow >= ROWS - 1) {
                    gameState->playerDirection = 0;
                } else {
                    gameState->grid[tailRow][tailColumn] = EMPTY;
                    gameState->snakeBody.pop();
                    headRow++;
                    if (gameState->grid[headRow][headColumn] == FOOD) {
                        gameState->spawnFood = true;
                    }
                    gameState->grid[headRow][headColumn] = SNAKE;
                    gameState->snakeBody.push({headRow, headColumn});
                }
                break;
            case LEFT:
                if (headColumn <= 0) {
                    gameState->playerDirection = 0;
                } else {
                    gameState->grid[tailRow][tailColumn] = EMPTY;
                    gameState->snakeBody.pop();
                    headColumn--;
                    if (gameState->grid[headRow][headColumn] == FOOD) {
                        gameState->spawnFood = true;
                    }
                    gameState->grid[headRow][headColumn] = SNAKE;
                    gameState->snakeBody.push({headRow, headColumn});
                }
                break;
        }
    }

    int randRow;
    int randCol;
    if (gameState->spawnFood) {
        do {
            randRow = getRandomInt(0, ROWS - 1);
            randCol = getRandomInt(0, COLUMNS - 1);
        } while (gameState->grid[randRow][randCol] == SNAKE);
        gameState->grid[randRow][randCol] = FOOD;
        gameState->spawnFood = false;
    }

    SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gameState->renderer);

    SDL_FRect tile;
    int r, g, b;
    float x, y;
    float w = (float)COLUMNSIZE;
    float h = (float)ROWSIZE;
    int tileValue;
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            x = (float)(j * COLUMNSIZE);
            y = (float)(i * ROWSIZE);

            tile = {x, y, w, h};


            tileValue = gameState->grid[i][j];
            switch (tileValue) {
                case EMPTY:
                    r = 64;
                    g = 64;
                    b = 64;
                    break;
                case SNAKE:
                    r = 0;
                    g = 255;
                    b = 0;
                    break;
                case FOOD:
                    r = 255;
                    g = 0;
                    b = 0;
                    break;
                default:
                    r = 255;
                    g = 255;
                    b = 255;
            }
            if (tileValue == SNAKE && (i == gameState->snakeBody.back().first) && (j == gameState->snakeBody.back().second)) {
                r = 34;
                g = 139;
                b = 34;
            }
            SDL_SetRenderDrawColor(gameState->renderer, r, g, b, SDL_ALPHA_OPAQUE);
            if (tileValue == EMPTY) {
                SDL_RenderRect(gameState->renderer, &tile);
            } else {
                SDL_RenderFillRect(gameState->renderer, &tile);
            }
        }
    }

    SDL_RenderPresent(gameState->renderer);
    // SDL_Delay(1000);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    GameState* gameState = static_cast<GameState*>(appstate);
    if (gameState) {
        delete gameState;
    }
}