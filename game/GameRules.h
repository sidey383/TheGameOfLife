#pragma once
#include <string>
#include <vector>
#include <set>
#include "../logger/Logger.h"

namespace gol {
    class GameRules {
    private:
        Logger logger = Logger("GameRules");
        std::set<int> brith;
        std::set<int> survival;
    public:
        GameRules();

        GameRules(GameRules const &);

        GameRules(std::set<int> brith, std::set<int> survival);

        explicit GameRules(std::string data);

        bool isActive(bool hasDot, int neighbors) const;

        std::set<int> getBrith() const;

        std::set<int> getSurvive() const;

        GameRules &operator=(GameRules const &);

    };
}

std::ostream &operator<<(std::ostream &out, gol::GameRules rules);

