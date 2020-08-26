#ifndef STRUCT_H
#define STRUCT_H

enum gameState {
    READY,
    PAUSE,
    END_BY_EDGE,
    END_BY_OBSTACLE,
    END_BY_ITSELF,
    RUNNING,
};

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct position {
    int xPos;
    int yPos;
    position();
    position(int, int);
    bool operator==(const position&);
};

#endif // STRUCT_H
