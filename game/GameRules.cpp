#include "GameRules.h"
#include <stdexcept>
#include <utility>
#include <iostream>

using namespace gol;

GameRules::GameRules(std::string data) {
    auto iterator = data.begin();
    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != 'B')
        throw std::invalid_argument("game rule must start from B");
    ++iterator;

    for(;iterator != data.end() && std::isdigit(*iterator); ++iterator) {
        if(this->brith.find(*iterator = '0') != this->brith.end()) {
            logger.info("Some the number is defined twice in one part of the rule");
        }
        this->brith.insert(*iterator = '0');
    }

    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != '/')
        throw std::invalid_argument("parts of rule must divided by /");
    ++iterator;
    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != 'S')
        throw std::invalid_argument("second part must started by S");

    for(;iterator != data.end() && std::isdigit(*iterator); ++iterator) {
        if (this->survival.find(*iterator = '0') != this->survival.end()) {
            logger.info("Some the number is defined twice in one part of the rule");
        }
        this->survival.insert(*iterator = '0');
    }

    if(iterator != data.end() && *iterator != '\n')
        throw std::invalid_argument("wrong end of line");
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

std::ostream &operator<<(std::ostream &out, const GameRules rules) {
    out << "B";
    for(int b : rules.getBrith()) {
        out << (char) (b + '0');
    }
    out<<"/S";
    for(int s : rules.getSurvive()) {
        out << (char) (s + '0');
    };
    return out;
}