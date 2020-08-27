#ifndef GAMEOPERATOR_H
#define GAMEOPERATOR_H

#include "board.h"
#include "snake.h"
#include "food.h"
#include "struct.h"

#include <vector>

class MainWindow;

class Game {
public:
    Game(MainWindow*);

    gameState getState();
    edgeMode getEdgeMode();
    int getStep();
    int getScore();

    Snake* getSnake();
    Food* getFood();
    std::vector<position>& getObstacles();

public slots:
    void setState(gameState);
    void setEdgeMode(edgeMode);
    void setNewFood();
    void setSnakeGrowth();

    void newGame();
    void gameOver(gameState);

    void addStep();
    void addScore();

    void saveGame();
    void loadGame();

private:
    MainWindow* parent;

    gameState state;
    edgeMode edge;

    int step;
    int score;

    std::vector<position> obstacles;
    Snake* snake;
    Food* food;
};

#endif // GAMEOPERATOR_H
