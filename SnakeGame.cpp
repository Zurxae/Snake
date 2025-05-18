#include "SnakeGame.hpp"

// Random number generator setup (global)
std::random_device rd;
std::mt19937 gen(rd());

SnakeGame::SnakeGame(const int window_width, const int window_height, const int rows, const int cols)
    : rows(rows), cols(cols) {

    // Initialize Window
    if (!initializeWindow(window_width, window_height)) {
        return;
    }

    // Set tile
    w = (float)(window_width / cols);
    h = (float)(window_height / rows);

    initialized = true;
    SDL_Log("Snake game initialized");
}

SnakeGame::~SnakeGame() {
    delete windowManager;

    SDL_Log("Deleted Snake game");
}

bool SnakeGame::initializeWindow(const int window_width, const int window_height) {
    windowManager = new SDLWindow("Snake", window_width, window_height);

    return windowManager->isInitialized();
}

bool SnakeGame::isInitialized() {
    return initialized;
}

void SnakeGame::setGame() {
    SDL_Log("Restarting Game");
    
    // Clear grid
    if (grid.empty()) {
        grid.resize(rows, std::vector<TileType>(cols, TileType::Empty));
    } else {
        for (auto& row : grid) {
            for (auto& tile: row) {
                tile = TileType::Empty;
            }
        }
    }

    // Set Snake
    while (!snakeBody.empty()) {
        snakeBody.pop();
    }

    int initSnakeLength = 2;
    for (int i = 0; i < initSnakeLength; i++) {
        int row = rows / 2;
        int col = (cols / 2) + i - 2;
        snakeBody.push({row, col});
        grid[row][col] = TileType::Snake;
    }
    snakeHead = snakeBody.back();
    snakeDirection = MoveDirection::NONE;
    spawnFood = true;

    restartGame = false;
}

void SnakeGame::updateGameState() {
    // Check if game needs to be reset
    if (restartGame) {
        setGame();
    }    

    placeFood();

    // Check if snake should move
    constexpr Uint64 moveDelay = 100;
    Uint64 now = SDL_GetTicks();

    if (now > lastMoveTime + moveDelay) {
        lastMoveTime = now;
        moveSnake();
    }

    // Draw
    drawGrid();
}

void SnakeGame::drawGrid() {
    SDL_Renderer* renderer = windowManager->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = (float)(j * w);
            float y = (float)(i * h);
            TileType tileType = grid[i][j];

            bool isHead = (snakeHead.first == i && snakeHead.second == j);
            drawTile(tileType, x, y, isHead);
        }
    }
    SDL_RenderPresent(renderer);
}

void SnakeGame::drawTile(TileType tileType, float x, float y, bool isHead) {
    // Choose Color Based on tile type
    int r, g, b;
    
    switch (tileType) {
        case TileType::Empty:
            r = 64; g = 64; b = 64;
            break;
        case TileType::Snake:
            if (isHead) {
                r = 34; g = 139; b = 34; // dark green
            } else {
                r = 0; g = 255; b = 0;
            }
            break;
        case TileType::Food:
            r = 255;
            g = 0;
            b = 0;
            break;
        default:
            r = 0;
            g = 0;
            b = 0;
    }

    // Draw Tile
    SDL_FRect tile = {x, y, w, h};
    SDL_Renderer* renderer = windowManager->getRenderer();

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    if (tileType == TileType::Empty) {
        SDL_RenderRect(renderer, &tile);
    } else {
        SDL_RenderFillRect(renderer, &tile);
    }
}

void SnakeGame::setDirection(SDL_Scancode moveDirection) {
    switch (moveDirection) {
        case SDL_SCANCODE_UP:
            snakeDirection = MoveDirection::UP;
            break;
        case SDL_SCANCODE_RIGHT:
            snakeDirection = MoveDirection::RIGHT;
            break;
        case SDL_SCANCODE_DOWN:
            snakeDirection = MoveDirection::DOWN;
            break;
        case SDL_SCANCODE_LEFT:
            snakeDirection = MoveDirection::LEFT;
            break;
    }
}

void SnakeGame::moveSnake() {
    int headRow = snakeHead.first;
    int headColumn = snakeHead.second;
    int tailRow = snakeBody.front().first;
    int tailColumn = snakeBody.front().second;

    int dRow = 0;
    int dCol = 0;

    switch (snakeDirection) {
        case MoveDirection::UP: dRow = -1; break;
        case MoveDirection::DOWN: dRow = 1; break;
        case MoveDirection::LEFT: dCol = -1; break;
        case MoveDirection::RIGHT: dCol = 1; break;
        default:
            snakeDirection = MoveDirection::NONE;
            return;
    }

    int newRow = headRow + dRow;
    int newCol = headColumn + dCol;

    if (
        (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) ||
        (grid[newRow][newCol] == TileType::Snake)
    ) {
        restartGame = true;
        return;
    }

    if (grid[newRow][newCol] == TileType::Food) {
        spawnFood = true;
    } else {
        grid[tailRow][tailColumn] = TileType::Empty;
        snakeBody.pop();
    }

    grid[newRow][newCol] = TileType::Snake;
    snakeBody.push({newRow, newCol});
    snakeHead = snakeBody.back();
}

// Function to get random integer between min and max (inclusive)
int getRandomInt(int min, int max) {
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

void SnakeGame::placeFood() {
    int randRow;
    int randCol;
    int tries = 0;
    if (spawnFood) {
        do {
            randRow = getRandomInt(0, rows - 1);
            randCol = getRandomInt(0, cols - 1);
            tries++;
        } while (grid[randRow][randCol] == TileType::Snake);
        grid[randRow][randCol] = TileType::Food;
        spawnFood = false;
        SDL_Log("tries to place food: %d", tries);
    }
}
