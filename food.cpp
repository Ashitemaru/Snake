#include "food.h"
#include "game.h"
#include "constant.h"

#include <vector>

Food::Food(Game* p)
    : parent(p) {
    srand(time(NULL));
    std::vector<position>& snakeBody = parent->getSnake()->getBody();
    std::vector<position>& obstacles = parent->getObstacles();
    while (true) {
        position tmpPos(rand() % TILE_NUMBER, rand() % TILE_NUMBER);
        if (
            std::find(snakeBody.begin(), snakeBody.end(), tmpPos) == snakeBody.end() &&
            std::find(obstacles.begin(), obstacles.end(), tmpPos) == obstacles.end()
        ) {
            pos = tmpPos;
            break;
        }
    }
    p->addScore();
}

int Food::getXPos() {
    return pos.xPos;
}

int Food::getYPos() {
    return pos.yPos;
}

position Food::getPos() {
    return pos;
}

void Food::setPos(position _pos) {
    pos = _pos;
}
