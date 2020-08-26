#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QObject>

class MainWindow;

class Board: public QWidget {

    Q_OBJECT

public:
    Board(MainWindow*);

    virtual void paintEvent(QPaintEvent*) override;

private:
    MainWindow* parent;
};

#endif // BOARD_H
