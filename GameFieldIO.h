#pragma once
#include "GameField.h"
#include "Logger.h"
namespace gol {
    class GameFieldIO {
        std::string path;
        Logger logger = Logger("GameFieldIO");

    public:
        explicit GameFieldIO(std::string path);

        GameField readField();

        void writeField(GameField);

    };
}
