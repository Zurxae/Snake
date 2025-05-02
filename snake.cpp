#include <iostream>
#include "snake.hpp"

Snake::Snake(const int numRows, const int numColumns)
    : numRows(numRows), numColumns(numColumns) {

    std::cout << "Snake game initialized" << std::endl;
}

Snake::~Snake() {

}