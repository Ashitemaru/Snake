#include "struct.h"

#include <iostream>

position::position(): xPos(0), yPos(0) {};

position::position(int x, int y) {
    xPos = x;
    yPos = y;
}

bool position::operator==(const position& b) {
    // std::cout << xPos << ' ' << yPos << std::endl;
    return xPos == b.xPos && yPos == b.yPos;
}
