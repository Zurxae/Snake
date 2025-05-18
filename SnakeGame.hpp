#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <queue>
#include <vector>
#include <SDL3/SDL.h>
#include "SDLWindow.hpp"

enum class TileType : uint8_t {
    Empty,
    Snake,
    Food
};

class SnakeGame {
public:
    SnakeGame(const int window_width, const int window_height, const int rows, const int cols);
    ~SnakeGame();

    bool initializeWindow(int window_width, int window_height);
    bool isInitialized();
    void setGame();
    void updateGameState();
    void drawGrid();
    void drawTile(TileType tileType, float x, float y, bool isHead);
    void moveSnake();

    bool restartGame = true;

private:
    SDLWindow* windowManager = nullptr;
    bool initialized = false;
    int rows;
    int cols;
    float w;
    float h;
    std::vector<std::vector<TileType>> grid;
    std::queue<std::pair<int, int>> snakeBody;
    std::pair<int, int> snakeHead;
};

#endif
