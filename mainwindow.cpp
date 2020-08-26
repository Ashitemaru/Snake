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
#include <QToolBar>
#include <QIcon>

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
        int y = (int)(event->y() - board->y() - toolBar->height() - GRID_SHIFT) / TILE_WIDTH;

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
    QMenu* gameMenu = new QMenu(tr("&Game"));

    startAction = new QAction(QIcon(":/icons/start.svg"), tr("&Start the game"), this);
    pauseAction = new QAction(QIcon(":/icons/pause.svg"), tr("&Pause the game"), this);
    continueAction = new QAction(QIcon(":/icons/continue.svg"), tr("&Continue playing game"), this);
    restartAction = new QAction(QIcon(":/icons/restart.svg"), tr("&Restart a new game"), this);
    saveAction = new QAction(QIcon(":/icons/save.svg"), tr("S&ave the game"), this);
    loadAction = new QAction(QIcon(":/icons/load.svg"), tr("&Load a saved game"), this);
    quitAction = new QAction(QIcon(":/icons/quit.svg"), tr("&Close the window"), this);

    startAction->setShortcut(Qt::CTRL | Qt::Key_E);
    pauseAction->setShortcut(Qt::CTRL | Qt::Key_R);
    continueAction->setShortcut(Qt::CTRL | Qt::Key_T);
    restartAction->setShortcut(Qt::CTRL | Qt::Key_N);
    saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    loadAction->setShortcut(Qt::CTRL | Qt::Key_L);
    quitAction->setShortcut(Qt::CTRL | Qt::Key_C);

    menuBar()->addMenu(gameMenu);

    gameMenu->addAction(startAction);
    gameMenu->addSeparator();
    gameMenu->addAction(pauseAction);
    gameMenu->addAction(continueAction);
    gameMenu->addSeparator();
    gameMenu->addAction(restartAction);
    gameMenu->addSeparator();
    gameMenu->addAction(saveAction);
    gameMenu->addAction(loadAction);
    gameMenu->addSeparator();
    gameMenu->addAction(quitAction);

    // Create menu bar

    toolBar = addToolBar("Tool Bar");

    toolBar->addAction(startAction);
    toolBar->addAction(pauseAction);
    toolBar->addAction(continueAction);
    toolBar->addAction(restartAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(loadAction);
    toolBar->addAction(quitAction);

    toolBar->setMovable(false);

    // Create tool bar

    timer = new QTimer(this);

    // Create timer

    startBtn = new QPushButton(tr("Start"), this);
    pauseBtn = new QPushButton(tr("Pause"), this);
    continueBtn = new QPushButton(tr("Continue"), this);
    restartBtn = new QPushButton(tr("Restart"), this);
    saveBtn = new QPushButton(tr("Save"), this);
    loadBtn = new QPushButton(tr("Load"), this);
    quitBtn = new QPushButton(tr("Quit"), this);

    startBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    pauseBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    continueBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    restartBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    saveBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    loadBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);
    quitBtn->setFixedSize(BTN_LENGTH, BTN_WIDTH);

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

    QFont* titleFont = new QFont;
    titleFont->setPixelSize(TITLE_SIZE);
    titleFont->setFamily("Monaco");

    boardTitle = new QLabel;
    boardTitle->setText(tr("<font color=darkred>Score Board </font>"));

    // These spaces are aimed to align the board

    boardTitle->setAlignment(Qt::AlignCenter);
    boardTitle->setFont(*titleFont);

    QFont* subTitleFont = new QFont;
    subTitleFont->setPixelSize(SUB_TITLE_SIZE);
    subTitleFont->setFamily("Monaco");

    stepTitle = new QLabel;
    stepTitle->setText(tr("Step"));
    stepTitle->setAlignment(Qt::AlignCenter);
    stepTitle->setFont(*subTitleFont);

    scoreTitle = new QLabel;
    scoreTitle->setText(tr("Score"));
    scoreTitle->setAlignment(Qt::AlignCenter);
    scoreTitle->setFont(*subTitleFont);

    // Create score board titles

    QFont* numberFont = new QFont;
    numberFont->setPixelSize(NUMBER_SIZE);
    numberFont->setFamily("Monaco");

    stepBoard = new QLabel;
    stepBoard->setText(std::to_string(game->getStep()).c_str());
    stepBoard->setAlignment(Qt::AlignCenter);
    stepBoard->setFont(*numberFont);

    scoreBoard = new QLabel;
    scoreBoard->setText(std::to_string(game->getScore()).c_str());
    scoreBoard->setAlignment(Qt::AlignCenter);
    scoreBoard->setFont(*numberFont);

    // Create score board numbers

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

    QVBoxLayout* generalBoard = new QVBoxLayout;

    generalBoard->addWidget(boardTitle);

    QHBoxLayout* stepBoardLayout = new QHBoxLayout;

    stepBoardLayout->addWidget(stepTitle);
    stepBoardLayout->addWidget(stepBoard);

    QHBoxLayout* scoreBoardLayout = new QHBoxLayout;

    scoreBoardLayout->addWidget(scoreTitle);
    scoreBoardLayout->addWidget(scoreBoard);

    generalBoard->addLayout(stepBoardLayout);
    generalBoard->addLayout(scoreBoardLayout);

    // Score board layout

    QVBoxLayout* btnLayout = new QVBoxLayout;

    btnLayout->addStretch();
    btnLayout->addLayout(generalBoard);
    btnLayout->addStretch();

    btnLayout->addWidget(startBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(pauseBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(continueBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(restartBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(saveBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(loadBtn);
    btnLayout->addSpacing(BTN_INTERVAL);
    btnLayout->addWidget(quitBtn);

    btnLayout->addStretch();

    btnLayout->setAlignment(Qt::AlignHCenter);

    // General layout of the right part

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

    QObject::connect(startAction, &QAction::triggered, startBtn, &QPushButton::click);
    QObject::connect(pauseAction, &QAction::triggered, pauseBtn, &QPushButton::click);
    QObject::connect(continueAction, &QAction::triggered, continueBtn, &QPushButton::click);
    QObject::connect(restartAction, &QAction::triggered, restartBtn, &QPushButton::click);
    QObject::connect(saveAction, &QAction::triggered, saveBtn, &QPushButton::click);
    QObject::connect(loadAction, &QAction::triggered, loadBtn, &QPushButton::click);
    QObject::connect(quitAction, &QAction::triggered, quitBtn, &QPushButton::click);

    // Action(Menu bar) signals

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

    startAction->setDisabled(game->getState() != gameState::READY);
    pauseAction->setDisabled(game->getState() != gameState::RUNNING);
    continueAction->setDisabled(game->getState() != gameState::PAUSE);
    restartAction->setDisabled(game->getState() == gameState::READY || game->getState() == gameState::RUNNING);
    saveAction->setDisabled(game->getState() != gameState::PAUSE);
    loadAction->setDisabled(game->getState() != gameState::READY);
    quitAction->setDisabled(false);

    // Action disabled

    stepBoard->setText(std::to_string(game->getStep()).c_str());
    if (
        game->getState() == gameState::END_BY_EDGE ||
        game->getState() == gameState::END_BY_OBSTACLE ||
        game->getState() == gameState::END_BY_ITSELF
    ) {
        scoreBoard->setText(("<font color=red>" + std::to_string(game->getScore()) + "</font>").c_str());
    } else {
        scoreBoard->setText(std::to_string(game->getScore()).c_str());
    }

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
