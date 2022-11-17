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

        GameRules();

        GameRules(std::set<int> brith, std::set<int> survival);

        explicit GameRules(std::string data);

        bool isActive(bool hasDot, int neighbors);

        friend std::ostream& operator<< (std::ostream &stream, const gol::GameRules &rules);

    };
}
