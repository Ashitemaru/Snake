#ifndef SNAKE_H
#define SNAKE_H

#include "struct.h"

#include <QObject>

#include <vector>
#include <queue>

class Game;

class Snake: public QObject {

    Q_OBJECT

public:
    Snake(Game*);

    std::vector<position>& getBody();
    direction getDirection();
    int getGrowth();

    void setDir(direction);
    void setNextDirection(direction);
    void setGrowth();
    void setGrowth(int);

    void move();

signals:
    void foodTouched();
    void edgeTouched();
    void bodyTouched();
    void obstaclesTouched();

private:
    Game* parent;

    std::vector<position> body;

    direction dir;
    std::queue<direction> nextDir;

    int growth;

    void judge(position&, edgeMode);
    void operateOutOfRange();
};

#endif // SNAKE_H
