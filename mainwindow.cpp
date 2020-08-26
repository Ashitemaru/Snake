#include "mainwindow.h"
#include "game.h"
#include "constant.h"
#include "struct.h"
#include "snake.h"
#include "board.h"

#include <QTimer>
#include <QObject>
#include <QColor>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QGraphicsEffect>
#include <QRect>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>

#include <qnamespace.h>

#include <vector>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    basicInit();
    initUi();
    initSignalSlot();

    // Initialization
}

MainWindow::~MainWindow() {};

Game* MainWindow::getGame() {
    return game;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_W: game->getSnake()->setNextDirection(direction::UP); break;
        case Qt::Key_S: game->getSnake()->setNextDirection(direction::DOWN); break;
        case Qt::Key_A: game->getSnake()->setNextDirection(direction::LEFT); break;
        case Qt::Key_D: game->getSnake()->setNextDirection(direction::RIGHT); break;
        default: break;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (game->getState() == gameState::READY) {
        int x = (int)(event->x() - board->x() - GRID_SHIFT) / TILE_WIDTH;
        int y = (int)(event->y() - board->y() - GRID_SHIFT) / TILE_WIDTH;

        // std::cout << event->x() << ' ' << event->y() << std::endl;

        std::vector<position>& snakeBody = game->getSnake()->getBody();
        if (std::find(snakeBody.begin(), snakeBody.end(), position(x, y)) != snakeBody.end()) return;

        // Cannot put obstacle on the body of snake

        if (x >= TILE_NUMBER || x < 0 || y >= TILE_NUMBER || y < 0) return;

        // Cannot put obstacle out of the board

        std::vector<position>& obs = game->getObstacles();
        std::vector<position>::iterator iter = std::find(obs.begin(), obs.end(), position(x, y));
        if (iter == obs.end()) {
            obs.push_back(position(x, y));
        } else {
            obs.erase(iter);
        }
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::basicInit() {
    setWindowTitle(tr("Retro Snaker"));
    resize(WINDOW_LENGTH, WINDOW_WIDTH);
    setFixedSize(width(), height());

    game = new Game(this);
    board = new Board(this);
}

void MainWindow::initUi() {
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(0, 0, this->width(), 80);

    QMenu* startMenu = menuBar->addMenu(tr("&Start"));
    QMenu* pauseMenu = menuBar->addMenu(tr("&Pause"));
    QMenu* continueMenu = menuBar->addMenu(tr("&Continue"));
    QMenu* restartMenu = menuBar->addMenu(tr("&Restart"));
    QMenu* saveMenu = menuBar->addMenu(tr("S&ave"));
    QMenu* loadMenu = menuBar->addMenu(tr("&Load"));

    startAction = new QAction(tr("&Start"), this);
    pauseAction = new QAction(tr("&Pause"), this);
    continueAction = new QAction(tr("&Continue"), this);
    restartAction = new QAction(tr("&Restart"), this);
    saveAction = new QAction(tr("S&ave"), this);
    loadAction = new QAction(tr("&Load"), this);

    startMenu->addAction(startAction);
    pauseMenu->addAction(pauseAction);
    continueMenu->addAction(continueAction);
    restartMenu->addAction(restartAction);
    saveMenu->addAction(saveAction);
    loadMenu->addAction(loadAction);

    menuBar->setNativeMenuBar(false);

    // Create menu bar

    timer = new QTimer(this);

    // Create timer

    startBtn = new QPushButton(tr("Start"), this);
    pauseBtn = new QPushButton(tr("Pause"), this);
    continueBtn = new QPushButton(tr("Continue"), this);
    restartBtn = new QPushButton(tr("Restart"), this);
    saveBtn = new QPushButton(tr("Save"), this);
    loadBtn = new QPushButton(tr("Load"), this);
    quitBtn = new QPushButton(tr("Quit"), this);

    // Create button

    QFont* font = new QFont;
    font->setBold(true);
    font->setPixelSize(CAPTION_SIZE);
    font->setFamily("Monaco");

    gameOverLabel = new QLabel(board);
    gameOverLabel->setText(tr("Game Over!"));
    gameOverLabel->setGeometry(QRect(
        CAPTION_START_X, CAPTION_START_Y,
        CAPTION_LENGTH, CAPTION_WIDTH
    ));
    gameOverLabel->setAlignment(Qt::AlignCenter);
    gameOverLabel->setFont(*font);
    gameOverLabel->show();
    gameOverLabel->setVisible(false);

    pauseLabel = new QLabel(board);
    pauseLabel->setText(tr("Game Paused"));
    pauseLabel->setGeometry(QRect(
        CAPTION_START_X, CAPTION_START_Y,
        CAPTION_LENGTH, CAPTION_WIDTH
    ));
    pauseLabel->setAlignment(Qt::AlignCenter);
    pauseLabel->setFont(*font);
    pauseLabel->show();
    pauseLabel->setVisible(false);

    // Create caption label

    stepBoard = new QLabel;
    std::string step("Step: ");
    step += std::to_string(game->getStep());
    stepBoard->setText(step.c_str());
    stepBoard->setAlignment(Qt::AlignLeft);
    stepBoard->show();

    scoreBoard = new QLabel;
    std::string score("Score: ");
    step += std::to_string(game->getScore());
    scoreBoard->setText(score.c_str());
    scoreBoard->setAlignment(Qt::AlignLeft);
    scoreBoard->show();

    // Create score board

    dieOfEdge = new QLabel(board);
    dieOfObstacle = new QLabel(board);
    dieOfItself = new QLabel(board);

    dieOfEdge->setText(tr("<font color=gray>Your snake tried to escape the board but failed =w=</font>"));
    dieOfObstacle->setText(tr("<font color=gray>Your snake tried to hide under the obstacle but died =w=</font>"));
    dieOfItself->setText(tr("<font color=gray>Your snake tried to bite itself and found itself poisonous =w=</font>"));

    dieOfEdge->setAlignment(Qt::AlignCenter);
    dieOfObstacle->setAlignment(Qt::AlignCenter);
    dieOfItself->setAlignment(Qt::AlignCenter);

    dieOfEdge->setVisible(false);
    dieOfObstacle->setVisible(false);
    dieOfItself->setVisible(false);

    dieOfEdge->setGeometry(QRect(
        DEATH_CAPTION_START_X, DEATH_CAPTION_START_Y,
        DEATH_CAPTION_LENGTH, DEATH_CAPTION_WIDTH
    ));
    dieOfObstacle->setGeometry(QRect(
        DEATH_CAPTION_START_X, DEATH_CAPTION_START_Y,
        DEATH_CAPTION_LENGTH, DEATH_CAPTION_WIDTH
    ));
    dieOfItself->setGeometry(QRect(
        DEATH_CAPTION_START_X, DEATH_CAPTION_START_Y,
        DEATH_CAPTION_LENGTH, DEATH_CAPTION_WIDTH
    ));

    QFont* deathCaptionFont = new QFont;
    deathCaptionFont->setPixelSize(DEATH_CAPTION_SIZE);
    deathCaptionFont->setFamily("Monaco");

    dieOfEdge->setFont(*deathCaptionFont);
    dieOfObstacle->setFont(*deathCaptionFont);
    dieOfItself->setFont(*deathCaptionFont);

    dieOfEdge->show();
    dieOfObstacle->show();
    dieOfItself->show();

    // Create death information

    QVBoxLayout* btnLayout = new QVBoxLayout;

    btnLayout->addStretch();
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(pauseBtn);
    btnLayout->addWidget(continueBtn);
    btnLayout->addWidget(restartBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(loadBtn);
    btnLayout->addWidget(quitBtn);

    btnLayout->addWidget(stepBoard);
    btnLayout->addWidget(scoreBoard);
    btnLayout->addStretch();

    // Button & Score board layout

    QHBoxLayout* boardLayout = new QHBoxLayout;

    board->setMaximumSize(GRID_WIDGET_WIDTH, GRID_WIDGET_WIDTH);
    board->setMinimumSize(GRID_WIDGET_WIDTH, GRID_WIDGET_WIDTH);
    board->resize(GRID_WIDGET_WIDTH, GRID_WIDGET_WIDTH);
    boardLayout->addWidget(board);

    // Board layout

    QHBoxLayout* generalLayout = new QHBoxLayout;

    generalLayout->addLayout(boardLayout);
    generalLayout->addSpacing(BOARD_BTN_INTERVAL);
    generalLayout->addLayout(btnLayout);

    // General layout

    QWidget* central = new QWidget();

    central->setLayout(generalLayout);
    setCentralWidget(central);

    // Final set

    return;
}

void MainWindow::initSignalSlot() {
    QObject::connect(game->getSnake(), &Snake::foodTouched, [=]() {
        this->game->setNewFood();
        this->game->setSnakeGrowth();
    });
    QObject::connect(game->getSnake(), &Snake::edgeTouched, [=](){this->game->gameOver(gameState::END_BY_EDGE);});
    QObject::connect(game->getSnake(), &Snake::bodyTouched, [=](){this->game->gameOver(gameState::END_BY_ITSELF);});
    QObject::connect(game->getSnake(), &Snake::obstaclesTouched, [=](){this->game->gameOver(gameState::END_BY_OBSTACLE);});
    // TODO: GameOver information

    // Snake signals

    QObject::connect(startBtn, &QPushButton::clicked, [=](){this->game->setState(gameState::RUNNING);});
    QObject::connect(pauseBtn, &QPushButton::clicked, [=](){this->game->setState(gameState::PAUSE);});
    QObject::connect(continueBtn, &QPushButton::clicked, [=](){this->game->setState(gameState::RUNNING);});
    QObject::connect(restartBtn, &QPushButton::clicked, [=]() {
        this->game->newGame();
        this->resetSignalSlot();
    });
    QObject::connect(saveBtn, &QPushButton::clicked, [=](){this->game->saveGame();});
    QObject::connect(loadBtn, &QPushButton::clicked, [=](){this->game->loadGame();});
    QObject::connect(quitBtn, &QPushButton::clicked, this, &QMainWindow::close);

    // Button signals

    QObject::connect(startAction, &QAction::triggered, [=](){this->game->setState(gameState::RUNNING);});
    QObject::connect(pauseAction, &QAction::triggered, pauseBtn, &QPushButton::click);
    QObject::connect(continueAction, &QAction::triggered, continueBtn, &QPushButton::click);
    QObject::connect(restartAction, &QAction::triggered, restartBtn, &QPushButton::click);
    QObject::connect(saveAction, &QAction::triggered, saveBtn, &QPushButton::click);
    QObject::connect(loadAction, &QAction::triggered, loadBtn, &QPushButton::click);

    // Action(Menu bar) signals
    // ???

    QObject::connect(timer, &QTimer::timeout, [=]() {
        if (this->game->getState() == gameState::RUNNING) this->game->getSnake()->move();
        this->update();
        this->updateUi();
    });
    timer->start(TIMER_INTERVAL);

    // Timers
}

void MainWindow::updateUi() {
    gameOverLabel->setVisible(
        game->getState() == gameState::END_BY_EDGE ||
        game->getState() == gameState::END_BY_OBSTACLE ||
        game->getState() == gameState::END_BY_ITSELF
    );

    dieOfEdge->setVisible(game->getState() == gameState::END_BY_EDGE);
    dieOfObstacle->setVisible(game->getState() == gameState::END_BY_OBSTACLE);
    dieOfItself->setVisible(game->getState() == gameState::END_BY_ITSELF);

    pauseLabel->setVisible(game->getState() == gameState::PAUSE);

    // Caption

    startBtn->setDisabled(game->getState() != gameState::READY);
    pauseBtn->setDisabled(game->getState() != gameState::RUNNING);
    continueBtn->setDisabled(game->getState() != gameState::PAUSE);
    restartBtn->setDisabled(game->getState() == gameState::READY || game->getState() == gameState::RUNNING);
    saveBtn->setDisabled(game->getState() != gameState::PAUSE);
    loadBtn->setDisabled(game->getState() != gameState::READY);

    // Button disabled

    std::string step("Step: ");
    step += std::to_string(game->getStep());
    stepBoard->setText(step.c_str());

    std::string score("Score: ");
    score += std::to_string(game->getScore());
    scoreBoard->setText(score.c_str());

    // Score board
}

void MainWindow::resetSignalSlot() {
    QObject::connect(game->getSnake(), &Snake::foodTouched, [=]() {
        this->game->setNewFood();
        this->game->setSnakeGrowth();
    });
    QObject::connect(game->getSnake(), &Snake::edgeTouched, [=](){this->game->gameOver(gameState::END_BY_EDGE);});
    QObject::connect(game->getSnake(), &Snake::bodyTouched, [=](){this->game->gameOver(gameState::END_BY_ITSELF);});
    QObject::connect(game->getSnake(), &Snake::obstaclesTouched, [=](){this->game->gameOver(gameState::END_BY_OBSTACLE);});
}
