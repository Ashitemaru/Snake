#ifndef FOOD_H
#define FOOD_H

#include "struct.h"

#include <QObject>

class Game;

class Food: public QObject {

    Q_OBJECT

public:
    Food(Game*);

    int getXPos();
    int getYPos();
    position getPos();

    void setPos(position);

private:
    Game* parent;

    position pos;
};

#endif // FOOD_H
