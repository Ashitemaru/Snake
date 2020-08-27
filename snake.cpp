#include "snake.h"
#include "struct.h"
#include "game.h"
#include "constant.h"

bool safePos(position& p) {
    return p.xPos >= SAFE_RANGE_START && p.xPos <= SAFE_RANGE_END &&
           p.yPos >= SAFE_RANGE_START && p.yPos <= SAFE_RANGE_END;
}

bool oppositeDirection(direction a, direction b) {
    switch (a) {
        case direction::UP: return b == direction::DOWN;
        case direction::DOWN: return b == direction::UP;
        case direction::LEFT: return b == direction::RIGHT;
        case direction::RIGHT: return b == direction::LEFT;
    }
}

bool outOfRange(position& p) {
    return p.xPos >= TILE_NUMBER || p.xPos < 0 || p.yPos >= TILE_NUMBER || p.yPos < 0;
}

Snake::Snake(Game* p)
    : parent(p) {
    srand(time(NULL));
    while (true) {
        position headInitPos(rand() % TILE_NUMBER, rand() % TILE_NUMBER);
        if (safePos(headInitPos)) {
            body.push_back(headInitPos);
            break;
        }
    }

    dir = direction(rand() % 4);
    switch (dir) {
        case direction::UP: body.push_back(position(body[0].xPos, body[0].yPos + 1)); break;
        case direction::DOWN: body.push_back(position(body[0].xPos, body[0].yPos - 1)); break;
        case direction::LEFT: body.push_back(position(body[0].xPos + 1, body[0].yPos)); break;
        case direction::RIGHT: body.push_back(position(body[0].xPos - 1, body[0].yPos)); break;
    }

    growth = 0;
};

std::vector<position>& Snake::getBody() {
    return body;
}

direction Snake::getDirection() {
    return dir;
}

int Snake::getGrowth() {
    return growth;
}

void Snake::setDir(direction d) {
    dir = d;
}

void Snake::setNextDirection(direction d) {
    nextDir.push(d);
}

void Snake::setGrowth() {
    growth += SNAKE_GROWTH;
}

void Snake::setGrowth(int g) {
    growth = g;
}

void Snake::move() {
    position oldHeadPos = body[0];

    direction newDir = dir;
    if (!nextDir.empty()) {
        newDir = nextDir.front();
        if (oppositeDirection(nextDir.front(), dir)) {
            newDir = dir;
        }
        nextDir.pop();
    }

    // Check legal move & set new direction

    switch (newDir) {
        case direction::UP: --body[0].yPos; break;
        case direction::DOWN: ++body[0].yPos; break;
        case direction::LEFT: --body[0].xPos; break;
        case direction::RIGHT: ++body[0].xPos; break;
    }
    dir = newDir;
    judge(body[0], parent->getEdgeMode());

    // Move head & judge

    if (!growth) body.pop_back();
    if (growth) growth--;
    body.insert(++body.begin(), oldHeadPos);

    // Move body

    parent->addStep();
    return;
}

void Snake::judge(position& headPos, edgeMode edge) {
    if (headPos == parent->getFood()->getPos()) emit foodTouched();

    if (outOfRange(headPos) && (edge == edgeMode::CLASSIC)) emit edgeTouched();
    if (outOfRange(headPos) && (edge == edgeMode::EDGELESS)) operateOutOfRange();

    if (std::find(++body.begin(), body.end(), headPos) != body.end()) emit bodyTouched();
    if (std::find(parent->getObstacles().begin(), parent->getObstacles().end(), headPos) != parent->getObstacles().end()) emit obstaclesTouched();
}

void Snake::operateOutOfRange() {
    body[0].xPos = (body[0].xPos + TILE_NUMBER) % TILE_NUMBER;
    body[0].yPos = (body[0].yPos + TILE_NUMBER) % TILE_NUMBER;
}
