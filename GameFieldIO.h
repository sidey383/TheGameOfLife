#pragma once
#include "GameField.h"
#include "Logger.h"

namespace gol {
    class GameFieldIO {
        std::string path;
        Logger logger = Logger("GameFieldIO");
        std::string fileHeader = "#Life 1.06\n";

        std::pair<int, int> readNumbers(char* buffer);

    public:
        explicit GameFieldIO(std::string path);

        GameField readField();

        GameField getDefault();

        void writeInFile(GameField);

    };
}
