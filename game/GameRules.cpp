#include "GameRules.h"
#include <stdexcept>
#include <utility>
#include <iostream>

using namespace gol;

GameRules::GameRules(std::string data) {
    int i = 0;
    if(data.empty()) {
        throw FileFormatException("no symbols in rule", data, 0, 0);
    }
    if(data[i] != 'B') {
        throw FileFormatException("game rule must start from B", data, i, i+1);
    }
    i++;

    for(; i < data.size() && std::isdigit(data[i]); ++i) {
        if(this->brith.find(data[i]  - '0') != this->brith.end()) {
            logger.info() << "number" << data[i] - '0' << " is defined twice in first part of the rule";
        }
        logger.debug() << "insert " << data[i]-'0' << " in brith";
        this->brith.insert(data[i] - '0');
    }

    if(i >= data.size()) {
        throw FileFormatException("not full rule", data, i, i+1);
    }
    if(data[i] != '/') {
        throw FileFormatException("parts of rule must divided by /", data, i, i+1);
    }
    ++i;
    if(i >= data.size()) {
        throw FileFormatException("not full rule", data, i, i+1);
    }
    if(data[i] != 'S') {
        throw FileFormatException("second part must started from S", data, i, i+1);
    }
    ++i;
    for(; i < data.size() && std::isdigit(data[i]); ++i) {
        if (this->survival.find(data[i] - '0') != this->survival.end()) {
            logger.info() << "number" << data[i] - '0' << " is defined twice in second part of the rule";
        }
        logger.debug() << "insert " << data[i]-'0' << " in survival";
        this->survival.insert(data[i] - '0');
    }
}

GameRules::GameRules(std::set<int> brith, std::set<int> survival): brith(std::move(brith)), survival(std::move(survival)) {}

GameRules::GameRules() {
    this->brith.insert(3);
    this->survival.insert(2);
    this->survival.insert(3);
}

bool GameRules::isActive(bool hasDot, int neighbors) const {
    if(hasDot)
        return this->survival.find(neighbors) != this->survival.end();
    else
        return this->brith.find(neighbors) != this->brith.end();
}

std::set<int> GameRules::getBrith() const {
    return this->brith;
}

std::set<int> GameRules::getSurvive() const {
    return this->survival;
}

GameRules &GameRules::operator=(const GameRules & rules) {
    this->brith = rules.brith;
    this->survival = rules.survival;
    return *this;
}

GameRules::GameRules(const GameRules &rules) {
    this->brith = rules.brith;
    this->survival = rules.survival;
}

std::ostream &operator<<(std::ostream &out, gol::GameRules rules) {
    out << "B";
    for(int b : rules.getBrith()) {
        out << (char) (b + '0');
    }
    out<<"/S";
    for(int s : rules.getSurvive()) {
        out << (char) (s + '0');
    }
    return out;
}
