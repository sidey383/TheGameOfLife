#pragma once
#include <string>
#include <vector>
#include <set>
#include "Logger.h"

namespace gol {
    class GameRules {
    private:
        std::set<int> brith;
        std::set<int> survival;
        Logger logger = Logger("GameRules");

    public:

        explicit GameRules();

        explicit GameRules(std::string data);

        bool isActive(bool hasDot, int neighbors);
    };
}
