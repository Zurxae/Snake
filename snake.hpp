#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SDL3/SDL.h>

class Snake {
public:
    Snake(int numRows, int numColumns);
    ~Snake();

    int* createGrid(int rows, int columns);
    int updateSnake(int snakeLength);

private:
    int snakeLength;
    int numRows;
    int numColumns;
    int* grid;
};

#endif