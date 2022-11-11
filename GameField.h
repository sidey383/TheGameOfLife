#ifndef THEGAMEOFLIFE_GAMEFIELD_H
#define THEGAMEOFLIFE_GAMEFIELD_H


class GameField {
public:
    GameField();

    void setDot(Dot);

    void tick();

    GameScreen getScreen(int x, int y, int width, int height)

};

class GameScreen {
public:

    bool hasDot(Dot);

};

struct Dot {
    int x;
    int y;
};


#endif
