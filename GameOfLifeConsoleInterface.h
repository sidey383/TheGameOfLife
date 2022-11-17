#pragma once
#include "gameoflife.h"

namespace gol {

    class GameOfLifeConsoleInterface {

        GameField field;
        Logger logger = Logger("Game");
        int iterationCount = 0;

    public:

        GameOfLifeConsoleInterface(GameField field);

        void start();

    };
}
