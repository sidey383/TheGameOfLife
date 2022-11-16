#include "GameRules.h"
#include <stdexcept>

using namespace gol;

GameRules::GameRules(std::string data) {
    auto iterator = data.begin();
    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != 'B')
        throw std::invalid_argument("game rule must start from B");
    ++iterator;

    for(;iterator != data.end() && std::isdigit(*iterator); ++iterator)
        this->brith.insert(*iterator);

    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != '/')
        throw std::invalid_argument("parts of rule must divided by /");
    ++iterator;
    if(iterator == data.end())
        throw std::invalid_argument("not full rule");
    if(*iterator != 'S')
        throw std::invalid_argument("second part must started by S");

    for(;iterator != data.end() && std::isdigit(*iterator); ++iterator)
        this->survival.insert(*iterator);

    if(iterator != data.end() && *iterator != '\n')
        throw std::invalid_argument("wrong end of line");
}

//TODO: add warning printing
GameRules::GameRules() {
    this->brith = {3};
    this->survival = {2, 3};
}

bool GameRules::isActive(bool hasDot, int neighbors) {
    if(hasDot)
        return this->survival.find(neighbors) != this->survival.end();
    else
        return this->brith.find(neighbors) != this->brith.end();
}
