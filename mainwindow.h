#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "board.h"

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QMouseEvent>

class MainWindow: public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() override;

    Game* getGame();

protected:
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void mousePressEvent(QMouseEvent*) override;

private:
    Game* game;

    QTimer* timer;

    QPushButton* startBtn;
    QPushButton* pauseBtn;
    QPushButton* continueBtn;
    QPushButton* restartBtn;
    QPushButton* saveBtn;
    QPushButton* loadBtn;
    QPushButton* quitBtn;

    QLabel* stepBoard;
    QLabel* scoreBoard;

    QLabel* gameOverLabel;
    QLabel* dieOfItself;
    QLabel* dieOfObstacle;
    QLabel* dieOfEdge;

    QLabel* pauseLabel;

    QMenuBar* menuBar;

    QAction* startAction;
    QAction* pauseAction;
    QAction* continueAction;
    QAction* restartAction;
    QAction* saveAction;
    QAction* loadAction;

    Board* board;

    void basicInit();
    void initUi();
    void initSignalSlot();

    void updateUi();
    void resetSignalSlot();
};

#endif // MAINWINDOW_H