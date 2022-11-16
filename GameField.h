#pragma once
#include <string>
#include "GameRules.h"
#include "GameField.h"

namespace gol {
    class GameField {
        bool *data{};
        int width;
        int height;
        std::string name;
        GameRules rules;
        Logger logger = Logger("GameFieldIO");
        friend class GameFieldIO;

    public:

        GameField(GameRules rules, std::string name, const bool *data, int width, int height);

        ~GameField();

        void setDot(int x, int y, bool val);

        bool getData(int x, int y);

        void tick();

        int getWidth();

        int getHeight();

        GameRules getRules();

        unsigned int getArrayPose(int x, int y);

    };
}
