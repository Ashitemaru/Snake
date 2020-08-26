#include "board.h"
#include "constant.h"
#include "struct.h"
#include "mainwindow.h"

#include <QPainter>
#include <QBrush>
#include <QPen>

Board::Board(MainWindow* p)
    : QWidget(p) {
    parent = p;
}

void Board::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    QPen gridPainter(Qt::lightGray, GRID_EDGE_WIDTH);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Pre settings

    p.setPen(gridPainter);
    p.setBrush(Qt::white);
    p.drawRect(GRID_SHIFT, GRID_SHIFT, TILE_NUMBER * TILE_WIDTH, TILE_NUMBER * TILE_WIDTH);

    gridPainter.setWidth(GRID_SEPARATOR_WIDTH);
    gridPainter.setStyle(Qt::DashLine);
    p.setPen(gridPainter);

    for (int i = 1; i < TILE_NUMBER; ++i)
        p.drawLine(
            TILE_WIDTH * i + GRID_SHIFT, GRID_SHIFT,
            TILE_WIDTH * i + GRID_SHIFT, TILE_NUMBER * TILE_WIDTH + GRID_SHIFT
        );

    for (int i = 1; i < TILE_NUMBER; ++i)
        p.drawLine(
            GRID_SHIFT, TILE_WIDTH * i + GRID_SHIFT,
            TILE_NUMBER * TILE_WIDTH + GRID_SHIFT, TILE_WIDTH * i + GRID_SHIFT
        );

    // Print grid

    p.setPen(Qt::black);
    if (parent->getGame()->getState() != gameState::READY) {
        p.setBrush(Qt::red);
        p.drawRect(
            TILE_WIDTH * parent->getGame()->getFood()->getXPos() + FOOD_INTERVAL + GRID_SHIFT,
            TILE_WIDTH * parent->getGame()->getFood()->getYPos() + FOOD_INTERVAL + GRID_SHIFT,
            FOOD_WIDTH, FOOD_WIDTH
        );
    }

    // Paint food

    std::vector<position> snakeBody = parent->getGame()->getSnake()->getBody();
    p.setBrush(Qt::darkGray);
    for (int i = 1; i < (int)snakeBody.size(); ++i)
        p.drawRect(
            TILE_WIDTH * snakeBody[i].xPos + SNAKE_INTERVAL + GRID_SHIFT,
            TILE_WIDTH * snakeBody[i].yPos + SNAKE_INTERVAL + GRID_SHIFT,
            SNAKE_WIDTH, SNAKE_WIDTH
        );

    // Paint snake body

    p.setPen(Qt::black);
    p.setBrush(Qt::lightGray);
    p.drawRect(
        TILE_WIDTH * snakeBody[0].xPos + SNAKE_INTERVAL + GRID_SHIFT,
        TILE_WIDTH * snakeBody[0].yPos + SNAKE_INTERVAL + GRID_SHIFT,
        SNAKE_WIDTH, SNAKE_WIDTH
    );

    // Paint snake head

    p.setPen(Qt::gray);
    p.setBrush(QBrush(Qt::gray, Qt::BDiagPattern));
    for (position obs: parent->getGame()->getObstacles()) {
        p.drawRect(
            TILE_WIDTH * obs.xPos + OBSTACLE_INTERVAL + GRID_SHIFT,
            TILE_WIDTH * obs.yPos + OBSTACLE_INTERVAL + GRID_SHIFT,
            OBSTACLE_WIDTH, OBSTACLE_WIDTH
        );
    }

    // Paint obstacles

    QWidget::paintEvent(event);
    return;
}
