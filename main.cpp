#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// TODO: Adjust speed of the snake
// TODO: 2-player mode
// TODO: ranking
// TODO: color customize
// TODO: color jianbian
// TODO: game mode(difficulty)
// TODO: life mode
