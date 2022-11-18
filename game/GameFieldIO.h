#pragma once
#include "GameField.h"
#include "../logger/Logger.h"

namespace gol {
    class GameFieldIO {
        std::string path;
        Logger logger = Logger("GameFieldIO");
        std::string fileHeader = "#Life 1.06";
        std::pair<int, int> readNumbers(char *buffer) const;

    public:
        explicit GameFieldIO(std::string path);

        GameField readField() const;

        static GameField getDefault(unsigned int number);

        void writeInFile(GameField&) const;

    };
}
