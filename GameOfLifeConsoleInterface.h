#pragma once
#include "game/gameoflife.h"

namespace gol {

    class GameOfLifeConsoleInterface {

        GameField field;
        Logger logger = Logger("Game");
        int iterationCount = 0;

    public:

        explicit GameOfLifeConsoleInterface(GameField &field);

        void start();

    };
}
