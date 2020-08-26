#include "game.h"
#include "snake.h"
#include "food.h"
#include "struct.h"
#include "constant.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QStringList>
#include <QString>
#include <QList>

#include <vector>
#include <iostream>

Game::Game(MainWindow* p)
    : parent(p)
    , state(gameState::READY)
    , step(0)
    , score(-SCORE_PER_FOOD)
    , obstacles()
    , snake(new Snake(this))
    , food(new Food(this)) {};

gameState Game::getState() {
    return state;
}

int Game::getStep() {
    return step;
}

int Game::getScore() {
    return score;
}

Snake* Game::getSnake() {
    return snake;
}

Food* Game::getFood() {
    return food;
}

std::vector<position>& Game::getObstacles() {
    return obstacles;
}

void Game::setState(gameState s) {
    state = s;
}

void Game::setNewFood() {
    delete food;
    food = new Food(this);
}

void Game::setSnakeGrowth() {
    snake->setGrowth();
}

void Game::newGame() {
    delete snake;
    delete food;
    obstacles.clear();
    state = gameState::READY;
    step = 0;
    score = -SCORE_PER_FOOD;

    snake = new Snake(this);
    food = new Food(this);
}

void Game::gameOver(gameState stat) {
    state = stat;
}

void Game::addStep() {
    ++step;
}

void Game::addScore() {
    score += SCORE_PER_FOOD;
}

void Game::saveGame() {
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setViewMode(QFileDialog::Detail);

    QString path = fileDialog->getSaveFileName(nullptr, "Save game", ".", "*.snakesave");

    if (!path.isEmpty()) {
        QFile finput(path);
        if (!finput.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Open failed.";
        }

        QTextStream inputStream(&finput);
        inputStream << "##########\n";
        for (position pos: obstacles) {
            inputStream << pos.xPos << ' ' << pos.yPos << '\n';
        }
        inputStream << "##########\n\n";

        // Write in obstacles

        inputStream << "##########\n";
        for (position pos: snake->getBody()) {
            inputStream << pos.xPos << ' ' << pos.yPos << '\n';
        }
        inputStream << "##########\n";
        inputStream << snake->getDirection() << ' ' << snake->getGrowth() << '\n';
        inputStream << "##########\n\n";

        // Write in snake

        inputStream << "##########\n";
        inputStream << food->getXPos() << ' ' << food->getYPos() << '\n';
        inputStream << "##########\n\n";

        // Write in food

        inputStream << "##########\n";
        inputStream << step << ' ' << score << '\n';
        inputStream << "##########\n\n";

        // write in step & score

        finput.close();
    }
}

void Game::loadGame() {
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setWindowTitle("Choose a saving to load a game.");
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("Game Saving(*.snakesave)");

    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }

    if (!fileNames.empty()) {
        QFile foutput(fileNames.front());
        if (!foutput.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Open failed.";
        }

        QString tmpString;

        tmpString = foutput.readLine();
        obstacles.clear();
        while (true) {
            tmpString = foutput.readLine();
            if (tmpString == "##########\n") break;
            QList<QString> posList = tmpString.split(' ');
            obstacles.push_back(position(posList[0].toInt(), posList[1].toInt()));
        }

        // Read & set obstacles

        tmpString = foutput.readLine();
        tmpString = foutput.readLine();

        snake->getBody().clear();
        while (true) {
            tmpString = foutput.readLine();
            if (tmpString == "##########\n") break;
            QList<QString> posList = tmpString.split(' ');
            snake->getBody().push_back(position(posList[0].toInt(), posList[1].toInt()));
        }

        tmpString = foutput.readLine();
        QList<QString> dirGrowthList = tmpString.split(' ');
        snake->setDir(direction(dirGrowthList[0].toInt()));
        snake->setGrowth(dirGrowthList[1].toInt());

        // Read & set snake

        tmpString = foutput.readLine();
        tmpString = foutput.readLine();
        tmpString = foutput.readLine();
        tmpString = foutput.readLine();

        QList<QString> foodPosList = tmpString.split(' ');
        food->setPos(position(foodPosList[0].toInt(), foodPosList[1].toInt()));

        // Read & set food

        tmpString = foutput.readLine();
        tmpString = foutput.readLine();
        tmpString = foutput.readLine();
        tmpString = foutput.readLine();

        QList<QString> scoreList = tmpString.split(' ');
        step = scoreList[0].toInt();
        score = scoreList[1].toInt();

        // Read & set score

        state = gameState::PAUSE;
    }
}
