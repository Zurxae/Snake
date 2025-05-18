#include <iostream>
#include "SnakeGame.hpp"

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

    restartGame = false;
}

void SnakeGame::updateGameState() {
    drawGrid();
}

void SnakeGame::drawGrid() {
    SDL_Renderer* renderer = windowManager->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    int x, y;
    TileType tileType;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            x = (float)(j * w);
            y = (float)(i * h);
            tileType = grid[i][j];
            
            drawTile(tileType, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}

void SnakeGame::drawTile(TileType tileType, float x, float y) {
    // Choose Color Based on tile type
    int r, g, b;
    
    switch (tileType) {
        case TileType::Empty:
            r = 64;
            g = 64;
            b = 64;
            break;
        case TileType::Snake:
            r = 0;
            g = 255;
            b = 0;
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