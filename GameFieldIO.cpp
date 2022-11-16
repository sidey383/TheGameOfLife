#include "GameFieldIO.h"
#include <stdexcept>
#include <fstream>

using namespace gol;

GameFieldIO::GameFieldIO(std::string path): path(path) {}

GameField GameFieldIO::readField() {
    GameRules rules;
    std::string name = path;
    int width = 128;
    int height = 128;

    std::ifstream input(path);
    char buffer[2048];
    input.getline(buffer, 2048);
    if(buffer != (std::string) "#Life 1.06\n")
        throw std::invalid_argument("wrong file header");
    //TODO: reading other data

    bool data[width*height];
    //TODO: fill data

    return GameField(rules, name, data, width, height);

}
